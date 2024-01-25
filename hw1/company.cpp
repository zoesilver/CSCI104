#include "company.hpp"
#include <iostream>
using namespace std;


// Initializes the tracker with n students and their 1-person companies.
CompanyTracker::CompanyTracker(int n)
{
  numCompanies = n;
  companies = new Company* [numCompanies];
  for (int i = 0; i < numCompanies; ++i)
      companies[i] = new Company ();
}

// Deallocates all dynamically allocated memory.
CompanyTracker::~CompanyTracker()
{
  for (int i=0; i<numCompanies; i++){    
    this->deleteCompanyAndParents(companies[i]);
  }
  delete [] companies;
}

void CompanyTracker::merge(int i, int j)
{
  //confirm in valid range of list and not the same and not in same company
  if (inRange(i)&&inRange(j)&&(i!=j)&&!inSameCompany(i,j)){
    //bigCompany1 and 2 are pointers to adresses of companies that have no parent unit (they are the biggest)
    Company* bigCompany1 = largestCompany(companies[i]);
    Company* bigCompany2 = largestCompany(companies[j]);
    //creates new object with bigCompany1 and bigCompany2 as children
    Company *newParent = new Company(bigCompany1,bigCompany2);
    //sets these companies parent to be newParent
    bigCompany1->parent=newParent;
    bigCompany2->parent=newParent;
  }

}

void CompanyTracker::split(int i)
{
  //checks if in range
  //also need to check if it is a 1-p company
  if(inRange(i)){
    //finds largest company i belongs to
    Company* original = largestCompany(companies[i]);
    //checks to see if original is a 1-person company
    if (original->merge1!=nullptr){
      original->merge1->parent=nullptr;
      original->merge2->parent=nullptr;
      delete original;
    }

  }
}

bool CompanyTracker::inSameCompany(int i, int j)
{
  //checks if in range
  // Im pretty sure we already check this before each time we call this function
  if (inRange(i)&&inRange(j)){
    if (largestCompany(companies[i]) == largestCompany(companies[j])) {
      return true;
  }
  }
  
  return false;
}

Company* CompanyTracker::largestCompany(Company* student){
  //base case
  if (student->parent==nullptr){
    return student;
  }
  //recursive case
  else{
    return largestCompany(student->parent);
  }
 }
 //check if i is in range
bool CompanyTracker::inRange(int i){
  if ((i>numCompanies-1)||(i<0)){
    return false;
  }
  return true;
}
void CompanyTracker::deleteCompanyAndParents(Company* student){
    //check if it does have a parent
    if (student->parent!=nullptr){
      //if so, recurse until reaching largest company
      deleteCompanyAndParents(student->parent);
      }

    //check if it has children 
    if (student->merge1!=nullptr){
      //if so, set children's parent to nullptr
        student->merge1->parent=nullptr;
        student->merge2->parent=nullptr;
        }
    delete student;
      
    }



