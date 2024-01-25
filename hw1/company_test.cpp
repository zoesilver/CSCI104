#include <iostream>
#include <string>
#include "company.hpp"
using namespace std;

int main (int argc, char* argv[]) {
    CompanyTracker tracker(5);
    tracker.merge(0,1);
    cout<<"Merging 0 and 1"<<endl;
    if (tracker.inSameCompany(0,1)){
        cout<<"0 and 1 are merged."<<endl;
    }
    else{
        cout<<"0 and 1 are not merged"<<endl;
        cout<<"Program failed."<<endl;
        return 0;
    }

    tracker.merge(2,3);
    cout<<"Merging 2 and 3"<<endl;
    if (tracker.inSameCompany(2,3)){
        cout<<"2 and 3 are merged."<<endl;
    }
    else{
        cout<<"2 and 3 are not merged"<<endl;
        cout<<"Program failed."<<endl;
        return 0;
    }

    tracker.merge(0,3);
    cout<<"Merging 0 and 3"<<endl;
    if (tracker.inSameCompany(0,1)){
        cout<<"0 and 3 are merged."<<endl;
    }
    else{
        cout<<"0 and 3 are not merged"<<endl;
        cout<<"Program failed."<<endl;
        return 0;
    }
    
    tracker.split(2);
    cout<<"Splitting 2."<<endl;
    if (!tracker.inSameCompany(2,1)){
        cout<<"2 is split."<<endl;

    }
    else{
        cout<<"2 is not split."<<endl;
        cout<<"Program failed."<<endl;
        return 0;
        }

    tracker.split(2);
    cout<<"Splitting 2."<<endl;
    if (!tracker.inSameCompany(2,3)){
        cout<<"2 is split."<<endl;

    }
    else{
        cout<<"2 is not split."<<endl;
        cout<<"Program failed."<<endl;
        return 0;
        }

    tracker.merge(2,4);
    cout<<"Merging 2 and 4"<<endl;
    if (tracker.inSameCompany(0,1)){
        cout<<"2 and 4 are merged."<<endl;
    }
    else{
        cout<<"2 and 4 are not merged"<<endl;
        cout<<"Program failed."<<endl;
        return 0;
    }

    tracker.split(0);
    cout<<"Spliting"<<endl;
    if (!tracker.inSameCompany(0,1)){
        
        cout<<"0 is split."<<endl;
    }
    else{
        cout<<"0 is not split"<<endl;
        cout<<"Program failed."<<endl;
        return 0;
    }
    //test what happens if you merge with itself
    tracker.merge(1,1);
    cout<<"Merging 1 and 1"<<endl;
    if (tracker.inSameCompany(1,1)){
        cout<<"1 and 1 are merged."<<endl;
    }
    else{
        cout<<"1 and 1 are not merged"<<endl;
        cout<<"Program failed."<<endl;
        return 0;
    }
    //test what happens if you merge two companies that are already merged
    tracker.merge(2,4);
    cout<<"Merging 2 and 4"<<endl;
    if (tracker.inSameCompany(2,4)){
        cout<<"2 and 4 are merged."<<endl;
    }
    else{
        cout<<"2 and 4 are not merged"<<endl;
        cout<<"Program failed."<<endl;
        return 0;
    
    //test if the companies are out of range
        tracker.merge(-1,12);
        cout<<"Merging -1 and 12."<<endl;

    //test if the company being split is out of range
        tracker.split(-5);
        cout<<"Splitting -5."<<endl;
    //test what happens when a one-person company is split
        tracker.split(3);
        cout<<"Splitting 3."<<endl;
    //test a big input of numbers
    CompanyTracker tracker2(100);
    for (int i=0; i<100; i++){
        tracker2.merge(0, i);        
    }
    for (int i=0; i<100; i++){
        for (int j=0; j<100; j++){
            if(!tracker2.inSameCompany(i,j)){
                cout<<"Program failed."<<endl;
                return 0;
            }
        }
    }
    }
    cout<<"Big test passed."<<endl;
    cout<<"All tests passed!"<<endl;
    return 0;
}
