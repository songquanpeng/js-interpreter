function printArray(array, len) {
    while (len > 0) {
        len = len - 1;
        output(array[len]);
    }
}

function selectionSort(arr, length) {
    for (let i = 0; i < length; i = i + 1) {
        let minIndex = i;
        for (let j = i; j < length; j = j + 1) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            let temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
        }
    }
    printArray(arr, len);
}

let arr = [2, 5, 17, 7, 19, 90, -9, 11, 1, 0, 10, -6];
let len = 12;
selectionSort(arr, len);