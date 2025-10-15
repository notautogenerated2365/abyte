#pragma once

#include <stdexcept>
#include <vector>
#include <string>
#include <iostream>

class abyte {
    using uc = unsigned char;

    private:
        uc data;
    
    public:
        // allows operator[] to return an object that, when modified, reflects changes in the abyte
        class bitproxy { 
            private:
                uc& data;
                int index;
            
            public:
                constexpr bitproxy(uc& data, int index) : data(data), index(index) {}

                // allows read access to the bit
                operator bool() const {
                    return (data >> index) & 1;
                }
                
                // allows write access to the bit (reflects changes in the abyte)
                bitproxy& operator=(bool value) {
                    if (value) data |= (1 << index);
                    else data &= ~(1 << index);
                    return *this;
                }
        };

        // allows operator() to return an object that, when modified, reflects changes in the abyte
        class bitsproxy {
            private:
                uc& data;
                int startIndex;
                int endIndex;
            
            public:
                constexpr bitsproxy(uc& data, int startIndex, int endIndex) : data(data), startIndex(startIndex), endIndex(endIndex) {}

                // allows read access to the bits
                operator std::vector<bool>() const {
                    std::vector<bool> x;

                    for (int i = startIndex; i < endIndex; i++) {
                        x.push_back((data >> i) & 1);
                    }

                    return x;
                }
                
                // allows write access to the bits (reflects changes in the abyte)  
                bitsproxy& operator=(const std::vector<bool> value) {
                    if (value.size() != endIndex - startIndex) {
                        throw std::runtime_error("length mismatch, cannot assign bits with operator()");
                    }

                    for (int i = 0; i < value.size(); i++) {
                        if (value[i]) data |= (1 << (startIndex + i));
                        else data &= ~(1 << (startIndex + i));
                    }

                    return *this;
                }
        };

        abyte() {}
        constexpr abyte(const uc x) : data{x} {}
        
        // allows read access to individual bits
        bool operator[](const int index) const {
            if (index < 0 || index > 7) {
                throw std::out_of_range("abyte operator[] index must be between 0 and 7");
            }

            return (data >> index) & 1;
        }

        // allows write access to individual bits
        bitproxy operator[](const int index) {
            if (index < 0 || index > 7) {
                throw std::out_of_range("abyte operator[] index must be between 0 and 7");
            }

            return bitproxy(data, index);
        }

        // allows read access to specific groups of bits
        std::vector<bool> operator()(const int startIndex, const int endIndex) const {
            if (startIndex < 0 || startIndex > 7 || endIndex < 0 || endIndex > 8 || startIndex > endIndex) {
                throw std::out_of_range("Invalid indices: startIndex=" + std::to_string(startIndex) + ", endIndex=" + std::to_string(endIndex));
            }

            std::vector<bool> x;

            for (int i = startIndex; i < endIndex; i++) {
                x.push_back((data >> i) & 1);
            }

            return x;
        }

        // allows write access to specific groups of bits
        bitsproxy operator()(const int startIndex, const int endIndex) {
            if (startIndex < 0 || startIndex > 7 || endIndex < 0 || endIndex > 8 || startIndex > endIndex) {
                throw std::out_of_range("Invalid indices: startIndex=" + std::to_string(startIndex) + ", endIndex=" + std::to_string(endIndex));
            }

            return bitsproxy(data, startIndex, endIndex);
        }

        // allows implicit conversion to unsigned char for reading
        constexpr operator uc() const {
            return data;
        }

        // allows implicit conversion to unsigned char for writing
        constexpr operator uc&() {
            return data;
        }
};