#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 50000 // 테스트할 데이터 개수 (5만 개)
#define RUN 32     // 팀 정렬에서 사용할 기본 덩어리 크기

// ==========================================
// 1. 삽입 정렬 (Insertion Sort)
// ==========================================
void insertionSort(int arr[], int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int temp = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > temp) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = temp;
    }
}

// ==========================================
// 2. 퀵 정렬 (Quick Sort)
// ==========================================
void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// ==========================================
// 3. 팀 정렬 (Tim Sort - 삽입+병합 하이브리드)
// ==========================================
void merge(int arr[], int l, int m, int r) {
    int len1 = m - l + 1, len2 = r - m;
    int *left = (int *)malloc(sizeof(int) * len1);
    int *right = (int *)malloc(sizeof(int) * len2);

    for (int x = 0; x < len1; x++) left[x] = arr[l + x];
    for (int x = 0; x < len2; x++) right[x] = arr[m + 1 + x];

    int i = 0, j = 0, k = l;
    while (i < len1 && j < len2) {
        if (left[i] <= right[j]) arr[k++] = left[i++];
        else arr[k++] = right[j++];
    }
    while (i < len1) arr[k++] = left[i++];
    while (j < len2) arr[k++] = right[j++];

    free(left);
    free(right);
}

void timSort(int arr[], int n) {
    // 1단계: RUN 크기만큼 쪼개서 삽입 정렬
    for (int i = 0; i < n; i += RUN) {
        int right = (i + RUN - 1 < n - 1) ? (i + RUN - 1) : (n - 1);
        insertionSort(arr, i, right);
    }
    // 2단계: 정렬된 덩어리들을 병합 정렬로 합치기
    for (int size = RUN; size < n; size = 2 * size) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = left + size - 1;
            int right = (left + 2 * size - 1 < n - 1) ? (left + 2 * size - 1) : (n - 1);
            if (mid < right) merge(arr, left, mid, right);
        }
    }
}

// ==========================================
// 메인 함수: 배열 생성 및 시간 측정
// ==========================================
int main() {
    int *arr1 = (int *)malloc(sizeof(int) * SIZE);
    int *arr2 = (int *)malloc(sizeof(int) * SIZE);
    int *arr3 = (int *)malloc(sizeof(int) * SIZE);

    srand(time(NULL));

    // 똑같은 랜덤 숫자 5만 개를 3개의 배열에 각각 넣음 (공정한 비교를 위해)
    for (int i = 0; i < SIZE; i++) {
        int num = rand() % 100000;
        arr1[i] = num;
        arr2[i] = num;
        arr3[i] = num;
    }

    clock_t start, end;
    double cpu_time_used;

    printf("데이터 개수: %d 개\n", SIZE);
    printf("----------------------------------\n");

    // 1. 삽입 정렬 시간 측정
    start = clock();
    insertionSort(arr1, 0, SIZE - 1);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("삽입 정렬 소요 시간: %f 초\n", cpu_time_used);

    // 2. 퀵 정렬 시간 측정
    start = clock();
    quickSort(arr2, 0, SIZE - 1);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("퀵 정렬 소요 시간:   %f 초\n", cpu_time_used);

    // 3. 팀 정렬 시간 측정
    start = clock();
    timSort(arr3, SIZE);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("팀 정렬 소요 시간:   %f 초\n", cpu_time_used);

    free(arr1); free(arr2); free(arr3);
    return 0;
}
