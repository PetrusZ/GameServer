/*
 * =====================================================================================
 *
 *       Filename:  binary_search.cc
 *
 *    Description:  binary_search
 *
 *        Version:  1.0
 *        Created:  04/16/2015 10:57:53 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Algorithm {
    public:
        int BinarySearch(const vector<int>& numbers, int target) {
            int begin = 0;
            int end = numbers.size() - 1;

            while(begin <= end) {
                int mid = (end + begin) / 2;

                if (numbers[mid] > target) {
                    end = mid - 1;
                } else if (numbers[mid] < target) {
                    begin = mid + 1;
                } else {
                    return mid;
                }
            }
            return -1;
        }

        void InsertSort(vector<int>& numbers) {
            int size = numbers.size();
            for (int i = 1; i < size; ++i) {
                int key = numbers[i];
                int j = i - 1;

                while(j >= 0 && numbers[j] > key) {
                    numbers[j + 1] = numbers[j];
                    --j;
                }
                numbers[j + 1] = key;
            }
        }

        void MergeSort(vector<int>& numbers, int left, int right) {
            if (left < right) {
                int mid = (left + right) / 2;
                MergeSort(numbers, left, mid);
                MergeSort(numbers, mid + 1, right);
                Merge(numbers, left, mid, right);
            }
        }

        void QuickSort(vector<int>& numbers, int left, int right) {
            if (left < right) {
                int mid = Partition(numbers, left, right);
                QuickSort(numbers, left, mid - 1);
                QuickSort(numbers, mid + 1, right);
            }
        }

    private:
        void Merge(vector<int>& numbers, int left, int mid, int right) {
            /*
             * 数组numbers[n], left、mid和right都是数组下标，满足left <= mid < right。
             * 假设数组numbers[left..mid]和numbers[mid+1..right]都已经排好序。函数合并
             * 这两个子数组形成单一的已排好序的子数组并代替当前的子数组numbers[left..right]。
             */

            vector<int> A, B;
            for (int i = left; i <= mid; ++i) {
                A.push_back(numbers[i]);
            }
            for (int j = mid + 1; j <= right; ++j) {
                B.push_back(numbers[j]);
            }

            int i = 0, j = 0;
            int a_size = A.size();
            int b_size = B.size();
            while(i < a_size && j < b_size) {
                if (A[i] < B[j]) {
                    numbers[left++] = A[i++];
                } else {
                    numbers[left++] = B[j++];
                }
            }

            for ( ; i < a_size; ++i) {
                numbers[left++] = A[i];
            }
            for ( ; j < b_size; ++j) {
                numbers[left++] = B[j];
            }
        }

        int Partition(vector<int>& numbers, int left, int right) {
            /*
             * 选择一个x=numbers[right]作为主元，并围绕它来划分子数组numbers[left..right]。
             * 最终分成小于等于主元的两个数组，numbers[left..mid-1]和A[mid+1..right]。
             */
            int pivot = numbers[right];
            int i = left - 1;
            for (int j = left; j < right; ++j) {
                if (numbers[j] < pivot) {
                    ++i;
                    int temp = numbers[j];
                    numbers[j] = numbers[i];
                    numbers[i] = temp;
                }
            }
            int temp = numbers[right];
            numbers[right] = numbers[i + 1];
            numbers[i + 1] = temp;

            return i + 1;
        }
};

int main(int argc, char *argv[])
{
    vector<int> numbers = {2, 4, 5, 6, 7, 3, 11, 23, 48, 9, 167, 4329, 1};
    Algorithm algorithm;
    // algorithm.MergeSort(numbers, 0, numbers.size() -1);
    algorithm.QuickSort(numbers, 0, numbers.size() -1);
    // algorithm.InsertSort(numbers);

    for_each(numbers.begin(), numbers.end(), [](int num) { cout << num << " "; });
    cout << endl;

    // cout << numbers.size() - 1 << endl;
    int result = algorithm.BinarySearch(numbers, 2);
    cout << result << endl;


    numbers = {};
    result = algorithm.BinarySearch(numbers, 0);
    cout << result << endl;
    return 0;
}
