#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class TimSort {
private:
    vector<int> arr;
    vector<int> original_arr;
    int minRun;

    int calculateMinRun(int n) {
        cout << "Вычисление minrun:" << endl;
        int r = 0;
        while (n >= 64) {
            r |= n & 1;
            n >>= 1;
        }
        int result = n + r;
        cout << "minrun = " << result << "\n" << endl;
        return result;
    }

    void insertionSort(int left, int right) {
        cout << "Сортировка вставками диапазона [" << left << ", " << right << "]" << endl;
        cout << "Исходный подмассив: ";
        for (int i = left; i <= right; i++) {
            cout << arr[i] << " ";
        }
        cout << endl;

        for (int i = left + 1; i <= right; i++) {
            int key = arr[i];
            int j = i - 1;

            while (j >= left && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }

        cout << "Результат сортировки вставками: ";
        for (int i = left; i <= right; i++) {
            cout << arr[i] << " ";
        }
        cout << "\n" << endl;
    }

    void findAndSortRuns() {
        cout << "Поиск и сортировка run:" << endl;
        int n = arr.size();
        int i = 0;

        while (i < n) {
            int j = i;
            if (j < n - 1) {
                bool increasing = arr[j] <= arr[j + 1];

                while (j < n - 1) {
                    if ((increasing && arr[j] <= arr[j + 1]) ||
                        (!increasing && arr[j] >= arr[j + 1])) {
                        j++;
                    }
                    else {
                        break;
                    }
                }

                if (!increasing) {
                    reverse(arr.begin() + i, arr.begin() + j + 1);
                    cout << "Найден убывающий run [" << i << ", " << j << "], развернули в возрастающий" << endl;
                }
                else {
                    cout << "Найден возрастающий run [" << i << ", " << j << "]" << endl;
                }
            }

            int currentRunLength = j - i + 1;
            if (currentRunLength < minRun) {
                int end = min(i + minRun - 1, n - 1);
                cout << "Run слишком короткий (" << currentRunLength << " < " << minRun
                    << "), расширяем до [" << i << ", " << end << "]" << endl;
                insertionSort(i, end);
                j = end;
            }
            else {
                cout << "Run достаточной длины, сортируем вставками" << endl;
                insertionSort(i, j);
            }

            i = j + 1;
        }
    }

    int gallopSearch(int value, const vector<int>& arr, int left, int right, bool findLeft) {
        int low = left;
        int high = right;

        while (low <= high) {
            int mid = low + (high - low) / 2;

            if ((findLeft && arr[mid] < value) || (!findLeft && arr[mid] <= value)) {
                low = mid + 1;
            }
            else {
                high = mid - 1;
            }
        }

        return findLeft ? low : high;
    }

    void mergeWithGallop(int left, int mid, int right) {
        cout << "Слияние run [" << left << ", " << mid << "] и [" << (mid + 1) << ", " << right << "]" << endl;

        cout << "Левый run: ";
        for (int i = left; i <= mid; i++) {
            cout << arr[i] << " ";
        }
        cout << endl;

        cout << "Правый run: ";
        for (int i = mid + 1; i <= right; i++) {
            cout << arr[i] << " ";
        }
        cout << endl;

        int len1 = mid - left + 1;
        int len2 = right - mid;

        vector<int> leftArr(len1);
        vector<int> rightArr(len2);

        for (int i = 0; i < len1; i++) {
            leftArr[i] = arr[left + i];
        }
        for (int i = 0; i < len2; i++) {
            rightArr[i] = arr[mid + 1 + i];
        }

        int i = 0, j = 0, k = left;
        int gallopCount = 0;
        const int GALLOP_THRESHOLD = 7;
        bool gallopActivated = false;

        cout << "Начало слияния..." << endl;

        while (i < len1 && j < len2) {
            if (leftArr[i] <= rightArr[j]) {
                arr[k] = leftArr[i];
                i++;
                gallopCount = (gallopCount > 0) ? gallopCount + 1 : 1;
            }
            else {
                arr[k] = rightArr[j];
                j++;
                gallopCount = (gallopCount < 0) ? gallopCount - 1 : -1;
            }
            k++;

            if (abs(gallopCount) >= GALLOP_THRESHOLD && !gallopActivated) {
                gallopActivated = true;
                cout << "\n>>> Активирован режим галопа! <<<" << endl;
                if (gallopCount > 0) {
                    cout << "Обнаружено " << gallopCount << " последовательных элементов из левого run" << endl;
                    cout << "Текущий элемент левого run: " << leftArr[i] << endl;
                    cout << "Поиск границы в правом run с помощью бинарного поиска..." << endl;

                    int gallopEnd = gallopSearch(leftArr[i], rightArr, j, len2 - 1, false);
                    int elementsToCopy = gallopEnd - j + 1;

                    cout << "Найдена граница: копируем " << elementsToCopy
                        << " элементов из правого run (индексы " << j << " до " << gallopEnd << ")" << endl;

                    for (int x = 0; x < elementsToCopy; x++) {
                        arr[k + x] = rightArr[j + x];
                    }
                    j += elementsToCopy;
                    k += elementsToCopy;

                    cout << "Режим галопа завершен. Пропущено " << elementsToCopy << " элементов" << endl;
                }
                else {
                    cout << "Обнаружено " << abs(gallopCount) << " последовательных элементов из правого run" << endl;
                    cout << "Текущий элемент правого run: " << rightArr[j] << endl;
                    cout << "Поиск границы в левом run с помощью бинарного поиска..." << endl;

                    int gallopEnd = gallopSearch(rightArr[j], leftArr, i, len1 - 1, true);
                    int elementsToCopy = gallopEnd - i + 1;

                    cout << "Найдена граница: копируем " << elementsToCopy
                        << " элементов из левого run (индексы " << i << " до " << gallopEnd << ")" << endl;

                    for (int x = 0; x < elementsToCopy; x++) {
                        arr[k + x] = leftArr[i + x];
                    }
                    i += elementsToCopy;
                    k += elementsToCopy;

                    cout << "Режим галопа завершен. Пропущено " << elementsToCopy << " элементов" << endl;
                }
                gallopCount = 0;
                cout << "Продолжение обычного слияния..." << endl;
            }
        }

        while (i < len1) {
            arr[k] = leftArr[i];
            i++;
            k++;
        }

        while (j < len2) {
            arr[k] = rightArr[j];
            j++;
            k++;
        }

        if (!gallopActivated) {
            cout << "Режим галопа не активирован (не было 7+ последовательных элементов)" << endl;
        }

        cout << "Результат слияния: ";
        for (int idx = left; idx <= right; idx++) {
            cout << arr[idx] << " ";
        }
        cout << "\n" << endl;
    }

    void initialInsertionSort() {
        cout << "Сортировка вставками:" << endl;
        int n = arr.size();

        for (int i = 0; i < n; i += minRun) {
            int end = min(i + minRun - 1, n - 1);
            cout << "Обрабатываем подмассив [" << i << ", " << end << "]" << endl;
            insertionSort(i, end);
        }
    }

    void mergeRuns() {
        int n = arr.size();

        for (int size = minRun; size < n; size = 2 * size) {
            cout << "Текущий размер слияния: " << size << endl;
            for (int left = 0; left < n; left += 2 * size) {
                int mid = left + size - 1;
                int right = min(left + 2 * size - 1, n - 1);

                if (mid < right) {
                    cout << "Сливаем run [" << left << ", " << mid << "] и [" << (mid + 1) << ", " << right << "]" << endl;
                    mergeWithGallop(left, mid, right);
                }
                else {
                    cout << "Run [" << left << ", " << mid << "] не требует слияния" << endl;
                }
            }
        }
        cout << "Завершено слияние всех run\n" << endl;
    }

public:
    TimSort(vector<int>& input) : arr(input), original_arr(input) {
        minRun = calculateMinRun(arr.size());
    }

    void sort() {
        cout << "Начальная сортировка вставками: " << endl << endl;
        arr = original_arr;
        initialInsertionSort();

        cout << "Поиск и сортировка run: " << endl << endl;
        arr = original_arr;
        cout << "Исходный массив: ";
        printArray();
        cout << endl;
        findAndSortRuns();

        cout << "Слияние run: " << endl;
        arr = original_arr;
        initialInsertionSort();
        findAndSortRuns();
        mergeRuns();

    }

    void printArray() {
        for (int num : arr) {
            cout << num << " ";
        }
        cout << endl;
    }
};

vector<int> inputArray() {
    vector<int> arr;
    int n;

    cout << "Введите количество элементов массива: ";
    cin >> n;

    cout << "Введите " << n << " элементов через пробел: ";
    for (int i = 0; i < n; i++) {
        int element;
        cin >> element;
        arr.push_back(element);
    }

    return arr;
}

int main() {
    setlocale(LC_ALL, "Russian");
    cout << "Лабораторная работа: Реализация Timsort" << endl;
    cout << endl;

    vector<int> arr = inputArray();

    cout << endl;
    cout << "Исходный массив: ";
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;

    vector<int> arr1 = arr;
    TimSort sorter(arr1);
    sorter.sort();

    cout << "\nИтоговый отсортированный массив: ";
    sorter.printArray();

    return 0;
}