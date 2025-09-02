#include <stdio.h>
#include <stdlib.h>

// Max Heap 比較函數（用於heap操作）
int heap_compare(int **target, int **parent)
{
    int a1 = (*target)[0], b1 = (*target)[1];
    int a2 = (*parent)[0], b2 = (*parent)[1];

    long long left = (long long)(b1 - a1) * b2 * (b2 + 1);
    long long right = (long long)(b2 - a2) * b1 * (b1 + 1);

    if (left > right)
        return 1; // target > parent (Max heap)
    if (left < right)
        return 0; // target < parent
    return 0;     // 相等
}

// 向上調整（插入時使用）
void heapify_up(int **heap, int index)
{
    while (index > 0)
    {
        int parent = (index - 1) / 2;

        if (heap_compare(&heap[index], &heap[parent]))
        {
            // 交換
            int *temp = heap[index];
            heap[index] = heap[parent];
            heap[parent] = temp;

            index = parent; // 繼續向上檢查
        }
        else
        {
            break; // 已經滿足heap性質，停止
        }
    }
}

// 向下調整（刪除時使用）
void heapify_down(int **heap, int size, int index)
{
    while (1)
    {
        int largest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < size && heap_compare(&heap[left], &heap[largest]))
            largest = left;

        if (right < size && heap_compare(&heap[right], &heap[largest]))
            largest = right;

        if (largest != index)
        {
            // 交換
            int *temp = heap[index];
            heap[index] = heap[largest];
            heap[largest] = temp;

            index = largest; // 繼續向下檢查
        }
        else
        {
            break; // 已經滿足heap性質，停止
        }
    }
}
// 建立初始heap
void build_heap(int **heap, int size)
{
    // 從最後一個非葉子節點開始向下調整
    for (int i = (size / 2) - 1; i >= 0; i--)
    {
        heapify_down(heap, size, i);
    }
}

// 插入元素到heap
void heap_push(int **heap, int *size, int *item)
{
    heap[*size] = item;
    heapify_up(heap, *size);
    (*size)++;
}

// 從heap取出最大元素
int *heap_pop(int **heap, int *size)
{
    if (*size == 0)
        return NULL;

    int *top = heap[0];

    // 把最後一個元素放到頂部
    heap[0] = heap[*size - 1];
    (*size)--;

    // 向下調整
    if (*size > 0)
        heapify_down(heap, *size, 0);

    return top;
}

// ===== Heap 實作結束 =====

// O(n + k log n) - 使用 heap 優化
double maxAverageRatio(int **classes, int classesSize, int *classesColSize, int extraStudents)
{
    printf("初始狀態：\n");
    for (int i = 0; i < classesSize; i++)
    {
        double a = classes[i][0];
        double b = classes[i][1];
        printf("[%d,%d] %f\n", classes[i][0], classes[i][1], (b - a) / (b * (b + 1)));
    }
    printf("\n");

    // 建立heap：O(n)
    int heap_size = classesSize;
    build_heap(classes, heap_size);

    printf("建立heap後：\n");
    for (int i = 0; i < classesSize; i++)
    {
        double a = classes[i][0];
        double b = classes[i][1];
        printf("[%d,%d] %f\n", classes[i][0], classes[i][1], (b - a) / (b * (b + 1)));
    }
    printf("\n");

    // k次操作：O(k log n)
    for (int i = 0; i < extraStudents; i++)
    {
        // 取出增益最大的班級
        int *best = heap_pop(classes, &heap_size);

        if (best == NULL)
            break;

        // 檢查是否已經100%
        if (best[0] == best[1])
        {
            heap_push(classes, &heap_size, best);
            break;
        }

        printf("第%d次操作：選擇班級 [%d,%d]\n", i + 1, best[0], best[1]);

        // 加入一個學生
        best[0]++;
        best[1]++;

        // 重新加入heap
        heap_push(classes, &heap_size, best);

        printf("更新後的heap頂部：[%d,%d]\n", classes[0][0], classes[0][1]);
        printf("\n");
    }

    // 計算最終平均值
    double total = 0.0;
    for (int i = 0; i < classesSize; i++)
        total += (double)classes[i][0] / classes[i][1];

    return total / classesSize;
}

int data[][2] = {
    {2, 4}, // gain = (4-2)/(4*5) = 2/20 = 0.1
    {3, 9}, // gain = (9-3)/(9*10) = 6/90 = 0.0667
    {4, 5}, // gain = (5-4)/(5*6) = 1/30 = 0.0333
    {2, 10} // gain = (10-2)/(10*11) = 8/110 = 0.0727
};

int *classes[] = {data[0], data[1], data[2], data[3]};
int classesSize = sizeof(classes) / sizeof(classes[0]);
int extraStudents = 4;

int main()
{

    double result = maxAverageRatio(classes, classesSize, NULL, extraStudents);
    printf("Result: %.5f\n", result);

    return 0;
}