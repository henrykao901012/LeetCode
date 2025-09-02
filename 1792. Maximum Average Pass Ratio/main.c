#include <stdio.h>
#include <stdlib.h>

int compare(const void *a, const void *b)
{
    int *arr1 = *(int **)a;
    int *arr2 = *(int **)b;

    // 最高優先：檢查是否 arr[0] == arr[1] (通過率100%)
    // int arr1_full = (arr1[0] == arr1[1]) ? 1 : 0; // 1表示100%通過率
    // int arr2_full = (arr2[0] == arr2[1]) ? 1 : 0;
    //
    // if (arr1_full != arr2_full)
    //{
    //    return arr1_full - arr2_full; // 100%的放後面
    //}

    // 第二優先：按照 (b-a)/(b(b+1)) 從大到小
    int a1 = arr1[0], b1 = arr1[1];
    int a2 = arr2[0], b2 = arr2[1];

    // 計算 (b1-a1)/(b1(b1+1)) 和 (b2-a2)/(b2(b2+1))
    // 為了避免浮點數運算，我們用交叉相乘比較
    // (b1-a1)/(b1(b1+1)) vs (b2-a2)/(b2(b2+1))
    // 等價於 (b1-a1)*b2*(b2+1) vs (b2-a2)*b1*(b1+1)

    long long left = (long long)(b1 - a1) * b2 * (b2 + 1);
    long long right = (long long)(b2 - a2) * b1 * (b1 + 1);

    if (left > right)
        return -1; // 從大到小，所以返回 -1
    if (left < right)
        return 1;
    return 0; // 相等
}

// O(nlgn + kn) too slow
double maxAverageRatio(int **classes, int classesSize, int *classesColSize, int extraStudents)
{
    qsort(classes, classesSize, sizeof(int *), compare);

    for (int i = 0; i < classesSize; i++)
    {
        double a = classes[i][0];
        double b = classes[i][1];
        printf("[%d,%d] %f\n", classes[i][0], classes[i][1], (b - a) / (b * (b + 1)));
    }
    printf("\n");

    for (int i = 0; i < extraStudents; i++)
    {
        classes[0][0]++;
        classes[0][1]++;

        // 將更新後的班級重新插入到正確的位置
        int j = 0;
        while (j < classesSize - 1 && compare(&classes[j], &classes[j + 1]) > 0)
        {
            int *temp = classes[j];
            classes[j] = classes[j + 1];
            classes[j + 1] = temp;
            j++;
        }

        for (int i = 0; i < classesSize; i++)
        {
            double a = classes[i][0];
            double b = classes[i][1];
            printf("[%d,%d] %f\n", classes[i][0], classes[i][1], (b - a) / (b * (b + 1)));
        }
        printf("\n");
    }

    double total = 0.0;
    for (int i = 0; i < classesSize; i++)
        total += (double)classes[i][0] / classes[i][1];

    return total / classesSize;
}

int data[][2] = {
    {2, 4},
    {3, 9},
    {4, 5},
    {2, 10}};

int *classes[] = {data[0], data[1], data[2], data[3]};
int classesSize = sizeof(classes) / sizeof(classes[0]);
int extraStudents = 4;

int main()
{
    double result = maxAverageRatio(classes, classesSize, NULL, extraStudents);
    printf("Result: %.5f\n", result);

    return 0;
}