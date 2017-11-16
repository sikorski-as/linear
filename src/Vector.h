#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
//#include <iostream>

namespace aisdi
{

template <typename Type>
class Vector
{
public:
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
    using value_type = Type;
    using pointer = Type*;
    using reference = Type&;
    using const_pointer = const Type*;
    using const_reference = const Type&;

    class ConstIterator;
    friend Vector<Type>::ConstIterator;
    class Iterator;
    friend Vector<Type>::Iterator;
    using iterator = Iterator;
    using const_iterator = ConstIterator;
private:
    pointer dataBlock;
    size_type count;
    size_type capacity;

    pointer allocBlock(size_type blockSize)
    {
        if(blockSize == 0)
        {
            return nullptr;
        }
        else
        {
            return new value_type [blockSize];
        }
    }

    void deallocBlock(pointer & dataBlock)
    {
        if(dataBlock)
        {
            delete [] dataBlock;
            dataBlock = nullptr;
        }
    }

    size_type newCapacity()
    {
        return capacity == 0 ? 8 : capacity * 2;
    }

    bool noSpace()
    {
        return count >= capacity;
    }

public:
    Vector() : dataBlock(nullptr), count(0), capacity(0) // lazy initialization, no memory allocated at the beginning
    {
        capacity = newCapacity();
        dataBlock = allocBlock(newCapacity());
    }

    Vector(std::initializer_list<Type> initList) : dataBlock(nullptr), count(0), capacity(0)
    {
        capacity = initList.size();
        dataBlock = allocBlock(capacity);
        for(const_reference listElement: initList)
        {
            append(listElement);
        }
    }

    Vector(const Vector& otherVector) : dataBlock(nullptr), count(0), capacity(0)
    {
        *this = otherVector;
    }

    Vector(Vector&& otherVector): dataBlock(otherVector.dataBlock), count(otherVector.count), capacity(otherVector.capacity)
    {
        otherVector.dataBlock = nullptr;
        otherVector.count = 0;
        otherVector.capacity = 0;
    }

    ~Vector()
    {
        deallocBlock(dataBlock);
    }

    Vector& operator=(const Vector& otherVector)
    {
        if(&otherVector == this)
            return *this;
        count = 0;
        capacity = otherVector.capacity;
        deallocBlock(dataBlock);
        dataBlock = allocBlock(capacity);
        for(auto elementIterator = otherVector.begin(); elementIterator != otherVector.end(); ++elementIterator)
        {
            append(*elementIterator);
        }
        return *this;
    }

    Vector& operator=(Vector&& otherVector)
    {
        count = otherVector.count;
        capacity = otherVector.capacity;
        deallocBlock(dataBlock);
        dataBlock = otherVector.dataBlock;

        otherVector.dataBlock = nullptr;
        otherVector.count = 0;
        otherVector.capacity = 0;

        return *this;
    }

    bool isEmpty() const
    {
        return count == 0;
    }

    size_type getSize() const
    {
        return count;
    }

    size_type getCapacity() const
    {
        return capacity;
    }

    void append(const Type& item)
    {
        if(noSpace())
        {
            pointer newDataBlock = allocBlock(newCapacity());
            for(size_type i = 0; i < count; i++)
            {
                newDataBlock[i] = dataBlock[i];
            }
            deallocBlock(dataBlock);
            dataBlock = newDataBlock;
            capacity = newCapacity();
        }
        dataBlock[count++] = item;
    }

    void prepend(const Type& item)
    {
        insert(begin(), item);
    }

    void insert(const const_iterator& insertPosition, const Type& item)
    {
        //std::cout << "====================================================\n";
        //std::cout << "index of the iterator " << insertPosition.index << "\n";
        //std::cout << "value to be placed inserted: " << item << "\n";
        //std::cout << "capacity:" << capacity << "\n";
        if(noSpace())
        {
            //std::cout << "no space!\n";
            pointer newDataBlock = allocBlock(newCapacity());
            for(size_type i = 0; i < insertPosition.index; i++)
            {
                newDataBlock[i] = dataBlock[i];
            }
            for(size_type i = count; i > insertPosition.index; i--)
            {
                newDataBlock[i] = dataBlock[i - 1];
            }
            newDataBlock[insertPosition.index] = item;
            deallocBlock(dataBlock);
            dataBlock = newDataBlock;
            capacity = newCapacity();
        }
        else
        {
            //std::cout << "there's space!\n";
            for(size_type i = count; i > insertPosition.index; i--)
            {
                dataBlock[i] = dataBlock[i - 1];
                //std::cout << "copying [" << i - 1 << " to " << i << "\n";
            }
            dataBlock[insertPosition.index] = item;
        }
        count++;
        //for(size_type i = 0; i < count; i++)
            //std::cout << "\t" << i << ": " << dataBlock[i] << "\n";
    }

    Type popFirst()
    {
        if(isEmpty())
            throw std::logic_error("Vector is empty");
        Type firstElement = dataBlock[0];
        for(size_type i = 0; i < count; i++)
        {
            dataBlock[i] = dataBlock[i + 1];
        }
        count--;
        return firstElement;
    }

    Type popLast()
    {
        if(isEmpty())
            throw std::logic_error("Vector is empty");
        return dataBlock[--count];
    }

    void erase(const const_iterator& position)
    {
        if(isEmpty() || position == end()) // throws std::out_of_range when: empty, end operator given
            throw std::out_of_range("Bad iterator");
        count--;
        for(size_type i = position.index; i < count; i++)
        {
            dataBlock[i] = dataBlock[i + 1];
        }
    }

    void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
    {
        size_type placeToInsert = firstIncluded.index;
        for(size_type i = lastExcluded.index; i < count; i++)
        {
            dataBlock[placeToInsert++] = dataBlock[i];
        }
        count -= (lastExcluded.index - firstIncluded.index);
    }

    iterator begin()
    {
        return ConstIterator(this, 0);
    }

    iterator end()
    {
        return ConstIterator(this, count);
    }

    const_iterator cbegin() const
    {
        return ConstIterator(this, 0);
    }

    const_iterator cend() const
    {
        return ConstIterator(this, count);
    }

    const_iterator begin() const
    {
        return cbegin();
    }

    const_iterator end() const
    {
        return cend();
    }
};

template <typename Type>
class Vector<Type>::ConstIterator
{
    friend Vector<Type>;
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename Vector::value_type;
    using difference_type = typename Vector::difference_type;
    using pointer = typename Vector::const_pointer;
    using reference = typename Vector::const_reference;
protected: // should (?) be available in inheriting classes
    const Vector<Type> * pointedVector;
    size_type index;

    ConstIterator(const Vector<Type> * initVector, size_type initIndex) : pointedVector(initVector), index(initIndex)
    {

    }

public:
    explicit ConstIterator() : pointedVector(nullptr), index(0) // what is it supposed to do? =end()?
    {

    }

    ConstIterator(const ConstIterator& other)
    {
        pointedVector = other.pointedVector;
        index = other.index;
    }

    reference operator*() const
    {
        if(index >= pointedVector->count)
            throw std::out_of_range("Bad iterator");
        return pointedVector->dataBlock[index];
    }

    ConstIterator& operator++()
    {
        if(index == pointedVector->count)
            throw std::out_of_range("Bad iterator");
        index++;
        return *this;
    }

    ConstIterator operator++(int)
    {
        if(index == pointedVector->count)
            throw std::out_of_range("Bad iterator");
        auto preObject = *this;
        index++;
        return preObject;
    }

    ConstIterator& operator--()
    {
        if(index == 0)
            throw std::out_of_range("Bad iterator");
        index--;
        return *this;
    }

    ConstIterator operator--(int)
    {
        if(index == 0)
            throw std::out_of_range("Bad iterator");
        auto preObject = *this;
        index--;
        return preObject;
    }

    ConstIterator operator+(difference_type d) const
    {
        auto preObject = *this;
        preObject.index += d;
        if(preObject.index < 0 || preObject.index > pointedVector->count)
            throw std::out_of_range("Bad iterator");
        return preObject;
    }

    ConstIterator operator-(difference_type d) const
    {
        auto preObject = *this;
        preObject.index -= d;
        if(preObject.index < 0 || preObject.index > pointedVector->count)
            throw std::out_of_range("Bad iterator");
        return preObject;
    }

    bool operator==(const ConstIterator& other) const
    {
        return pointedVector == other.pointedVector && index == other.index;
    }

    bool operator!=(const ConstIterator& other) const
    {
        return pointedVector != other.pointedVector || index != other.index;
    }
};

template <typename Type>
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
    friend Vector<Type>;
    using pointer = typename Vector::pointer;
    using reference = typename Vector::reference;
public:
    explicit Iterator()
    {}

    Iterator(const ConstIterator& other)
        : ConstIterator(other)
    {}

    Iterator& operator++()
    {
        ConstIterator::operator++();
        return *this;
    }

    Iterator operator++(int)
    {
        auto result = *this;
        ConstIterator::operator++();
        return result;
    }

    Iterator& operator--()
    {
        ConstIterator::operator--();
        return *this;
    }

    Iterator operator--(int)
    {
        auto result = *this;
        ConstIterator::operator--();
        return result;
    }

    Iterator operator+(difference_type d) const
    {
        return ConstIterator::operator+(d);
    }

    Iterator operator-(difference_type d) const
    {
        return ConstIterator::operator-(d);
    }

    reference operator*() const
    {
        // ugly cast, yet reduces code duplication.
        return const_cast<reference>(ConstIterator::operator*());
    }
};

} // namespace aisdi

#endif // AISDI_LINEAR_VECTOR_H
