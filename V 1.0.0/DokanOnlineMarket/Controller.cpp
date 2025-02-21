#include "Controller.h"
#include <cctype>

/**
 * @brief Controller::Controller  -> this is the constractor of Controller class
 * @param data -> it pointer of model class to connect data with controller
 */
Controller::Controller(Model *data)
{
    this->data = data;
}

Controller::~Controller() {}


/**
 * @brief Controller::LogSeller ->> this function take email and password of seller and search of them in data to know
 * if he can login or not
 * @param Email  ->> email of seller
 * @param Password ->> password of seller
 */
void Controller::LogSeller(string Email, string Password)
{

    if (data->SellerArr[Email].Email==Email && data->SellerArr[Email].Password == Password)
    {
        // make pointer "s" point to currunt seller by give the pointer his reference with "&" mark
        s = &data->SellerArr[Email];
        return;
    }
    // set the pointer with null if we not found data for seller who try to login
    s = nullptr;
}

/**
 * @brief Controller::LogCustomer ->> this function take email and password of Customer and search of them in data to know
 * if he can login or not
 * @param Email  ->> email of Customer
 * @param Password ->> password of Customer
 */
void Controller::LogCustomer(string Email, string Password)
{
    // make pointer "c" point to currunt Customer by give the pointer his reference with "&" mark
    if (data->CustomerArr[Email].Email == Email && data->CustomerArr[Email].Password == Password)
    {
        c = &data->CustomerArr[Email];
        return;
    }
    // set the pointer with null if we not found data for Customer who try to login
    c = nullptr;
}

/**
 * @brief Controller::RegisterSeller ->> this function take data of Seller to create for him new account in our system
 * by saving him data
 */
void Controller::RegisterSeller(string FirstName, string SecondName, string Phone, string Email, string Password, string Gender)
{
    // Create object of Seller
    Seller new_seller(data->CountSeller++, FirstName, SecondName, Phone, Email, Password, Gender);

    // add the object to "SellerArr"
    data->SellerArr[Email] = new_seller;

    // make pointer "s" point to currunt seller by give the pointer his reference with "&" mark
    s = &data->SellerArr[Email];
}

/**
 * @brief Controller::RegisterCustomer ->> this function take data of Customer to create for him new account in our system
 * by saving him data
 */
void Controller::RegisterCustomer(string FirstName, string SecondName, string PhoneNumber, string Gender, string Email, string Password)
{
    // Create object of Customer
    Customer new_customer(data->CountCustomer++, FirstName, SecondName, PhoneNumber, Gender, Email, Password);

    // add the object to "CustomerArr"
    data->CustomerArr[Email] = new_customer;

    // make pointer "c" point to currunt Customer by give the pointer his reference with "&" mark
    c = &data->CustomerArr[Email];
}

/**
 * @brief Controller::AddProduct  ->> take data of product to create new product and adding him to our data
 */
void Controller::AddProduct(string Name, float Price, int Quantity, string Description, string Category, int Offer_Percentage,
                            int NoOfDeliveryDays, string PathOfPhoto, string Seller_mail)
{
    // creat new pointer of Product class and give him his data
    Product *New_Product = new Product(data->CountProduct++, Name, Price, Quantity, Description, Category, Offer_Percentage, NoOfDeliveryDays, Seller_mail);
    New_Product->PathOfPhoto = PathOfPhoto;
    New_Product->CalculatePrice();

    // add the new product to all products container "ProductArr2"
    data->ProductArr2[New_Product->ID]=New_Product;

    // add the new product to list of selled product of it seller "SellerArr"
    data->SellerArr[New_Product->Seller_mail].SelledProducts[New_Product->ID] = data->ProductArr2[New_Product->ID];

    // add the new product to list of category which this product belongs to "CategoryArr"
    data->CategoryArr[New_Product->Category].push_back(data->ProductArr2[New_Product->ID]);
}


/**
 * @brief Controller::RemoveProduct -->> this function remove product fron any list in can be
 * found in it to delete it finaly from system
 * @param ID -->> it is the id of product what we want to delete
 */
void Controller::RemoveProduct(int ID)
{
    s->RemoveProduct(ID); // delete from seller list

    unordered_map<string, Customer>::iterator it;
    for (it=data->CustomerArr.begin() ; it!=data->CustomerArr.end() ; it++)
    {
        it->second.RemoveProductFromFavorite(ID); // delete from Favorite in each customer
        it->second.My_Cart.RemoveProduct(ID); // delete from Cart in each customer
    }

    data->ProductArr2.erase(ID); // delete from all products
}

/**
 * @brief Controller::CkeckNumber -->> this function take string and check if each char of string is number or not
 * @param Number ->> the string which we check it
 * @return boolin that say true if it is composed of number or false if not number
 */
bool Controller::CkeckNumber(string Number)
{
    for (int i=0 ; i<Number.size() ; i++)
    {
        // compare each char with specific ascii to know if number or not
        if (Number[i]<43 || Number[i]>57) return false;
    }
    return true;
}


void Controller::Sort(string type, vector<Product*> &p)
{
    if (type == "Name")
        sort(p.begin(), p.end(), CompareName());
    else if (type == "PriceUp")
        sort(p.begin(), p.end(), ComparePriceUp());
    else if (type == "PriceDown")
        sort(p.begin(), p.end(), ComparePriceDown());
    else if (type == "RateUp")
        sort(p.begin(), p.end(), CompareRateUp());
    else if (type == "RateDown")
        sort(p.begin(), p.end(), CompareRateDown());

}

void Controller::Search(string Name, vector<Product*> pro)
{
    data->ProductArrView.clear();
    for (int i=0 ; i< pro.size() ; i++)
    {
        if (pro[i]->Name.find(Name) != -1)
        {
            data->ProductArrView.push_back(pro[i]);
        }
    }
}
void Controller::ClearSatck()
{
    while (!data->BackToPage.empty())
    {
        data->BackToPage.pop();
    }
}
void Controller::ReloadData()
{
    for (int i=0 ; i<14 ; i++)
    {
        data->CategoryArr[data->CategoryName[i]].clear();
    }

    for (auto& it : data->ProductArr2)
    {
        data->CategoryArr[it.second->Category].push_back(it.second);
    }
}
string Controller::GetDate(int num)
{
    string date = QDate::currentDate().toString("dd:MM").toStdString();
    string day1 = "", month1="";
    day1+=date[0];
    day1+=date[1];
    month1+=date[3];
    month1+=date[4];
    int day=stoi(day1), month=stoi(month1);
    day += num;
    if (day > data->NumOfDaysIMonth[month-1])
    {
        day-=data->NumOfDaysIMonth[month-1];
        if (month==12) month=1;
        else month++;
    }

    return to_string(day) +" "+ data->months[month-1];
}
