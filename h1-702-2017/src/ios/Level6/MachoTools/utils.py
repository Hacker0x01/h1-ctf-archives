from bisect import bisect_left, insort_left

def in_list(arr, elem):
    idx = bisect_left(arr, elem)
    if idx >= len(arr):
        return False
    return arr[idx] == elem

def add_to_list(arr, elem):
    if not in_list(arr, elem):
        insort_left(arr, elem)
