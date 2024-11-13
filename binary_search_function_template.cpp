#include<iostream>
using namespace std;

template <typename T>
int _binary_search(T *arr , int n , T target)
{
    int l = 0, r = n-1, mid;

    while(l <= r)
    {
        mid = (l+r)/2;
        if(arr[mid] < target)
        {
            l = mid+1;
        }
        else if(arr[mid] > target)
        {
            r = mid-1;
        }
        else
        {
            return mid;
        }
    }
    return -1;

}


int main()
{

    char cT;
    int n,q;
    cin >> cT >> n >> q; // 按照題目輸入每筆測資的三個值

    if(cT == 'i')// 整數的情況
    {
        int arr[n] , search_arr[q];
        for(int i = 0 ; i < n ; i++)
        {
            cin >> arr[i];
        }
        for(int i = 0 ; i < q ; i++)
        {
             int target;
             cin >> target;
            cout << _binary_search<int>(arr,n,target) << endl;
        }
    }
    else if(cT == 's')// 字串的情況
    {
        string arr[n] , search_arr[q];
        for(int i = 0 ; i < n ; i++)
        {
            cin >> arr[i];
        }
        for(int i = 0 ; i < q ; i++)
        {
            string target;
            cin >> target;
            cout << _binary_search<string>(arr,n,target) << endl;
        }
    }

    return 0;
}
