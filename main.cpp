#include <iostream>


template <typename T>
class unique_pointer {

    T* data_;

public:

    explicit unique_pointer(T data)
    try :
        data_(new T(data)) {}
    catch(...) {
        delete data_;
        throw;
    }
    unique_pointer() : data_(nullptr) {}
    ~unique_pointer() {
        delete data_;
    }

    //delete default copy constructor and assignment operator
    unique_pointer(const unique_pointer&) = delete;
    unique_pointer& operator= (unique_pointer&) = delete;

    //move constructor
    unique_pointer(unique_pointer&& object) : unique_pointer() {
        std::swap(*this, object);
    }

    //move assignment
    unique_pointer& operator= (unique_pointer&& object){
        unique_pointer<T> temp(std::move(object));
        std::swap(*this,temp);
        return *this;
    }



    T* getData() {
        return data_;
    }
};


template <typename T>
class shared_pointer {

    T* data_;
    int* count_;

public:

    ~shared_pointer(){
        --(*count_);
        if(*count_ == 0){
            delete data_;
        }
    }

    shared_pointer()
    try :
        data_(nullptr),
        count_(new int(1)) { }
    catch(...) {
        delete count_;
        throw;
    }

    explicit shared_pointer(T data)
    try :
        data_(new int(data)),
        count_(new int(1)) { }
    catch(...) {
        delete data_;
        delete count_;
        throw;
    }

    shared_pointer(const shared_pointer& object) :
        data_(object.data_),
        count_(object.count_)
        {
            ++(*count_);
        }

    shared_pointer& operator= (const shared_pointer& object){

        // check for self-assignment
        if(this == &object){
            return *this;
        }

        T* old_data = data_;
        int* old_count = count_;

        data_ = object.data_;
        count_ = object.count_;
        ++(*count_);

        --(*old_count);
        if(*old_count == 0){
            delete old_data;
            delete old_count;
        }

        return *this;
    }

    int getCount() {
        return *count_;
    }

    T* getData() {
        return data_;
    }

    void setData(T data) {
        if(data_){
            *data_ = data;
        } else {
            data_ = new int(data);
        }
    }
};






int main() {

    unique_pointer<int> obj1;
    unique_pointer<int> obj2(3);

    if(obj1.getData()){
        std::cout << *(obj1.getData()) << std::endl;
    }

    if(obj2.getData()){
        std::cout << *(obj2.getData()) << std::endl;
    }

    std::cout << "###########################" << std::endl;

    std::cout << "\nsp1" << std::endl;
    shared_pointer<int> sp1;
    if(sp1.getData()){
        std::cout << *(sp1.getData()) << std::endl;
    }

    std::cout << "\nsp2" << std::endl;
    shared_pointer<int> sp2(2);
    if(sp2.getData()){
        std::cout << *(sp2.getData()) << std::endl;
    }

    std::cout << "\nsp3" << std::endl;
    shared_pointer<int> sp3(sp1);
    if(sp3.getData()){
        std::cout << *(sp3.getData()) << std::endl;
    }

    std::cout << "set sp1 " << std::endl;
    sp1.setData(5);

    std::cout << "\nre-assign sp2" << std::endl;
    sp2 = sp1;
    if(sp2.getData()){
        std::cout << *(sp2.getData()) << std::endl;
    }

    {
        shared_pointer<int> sp4(sp2);
    }

    return 0;
}