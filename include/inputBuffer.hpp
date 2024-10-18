#include <cinttypes>
#include <cstddef>
#include <istream>
#include <fstream>


namespace ast {

template<uint32_t SIZE = 512>
class InputBuffer {
    uint32_t used_;
    size_t globalPosition_;
    uint32_t pos_;
    std::istream *input_;
    char buffer_[SIZE];

    public:

    InputBuffer(std::istream *input)
        : used_(0), globalPosition_(0), pos_(0),
        input_(input),
        buffer_(0)
    {
        input_->read(buffer_, SIZE);
        used_ = input_->gcount();
    }
    InputBuffer()
        : used_(0), globalPosition_(0), pos_(0),
          input_(nullptr), buffer_(0)
    {}

    void openStream(std::istream *input) {
        input_ = input;
        input_->read(buffer_, SIZE);
        used_ = input_->gcount();
    }

    char get() {return buffer_[pos_];}

    InputBuffer &next(char &n) {
        n = buffer_[pos_++];
        if (pos_ >= used_) {
            if (!*input_) {
                pos_ = used_;
                return *this;
            }
            pos_ = 0;
            input_->read(buffer_, SIZE);
            globalPosition_ += used_;
            used_ = input_->gcount();
        }
        return *this;
    }

    InputBuffer &next() {
        pos_++;
        if (pos_ >= used_) {
            if (!*input_) {
                pos_ = used_;
                return *this;
            }
            pos_ = 0;
            input_->read(buffer_, SIZE);
            globalPosition_ += used_;
            used_ = input_->gcount();
        }
        return *this;
    }

    bool eof() {return pos_ == used_;}

    bool is_open() {return input_;}
    void close() {
        input_ = nullptr;
        used_ = 0;
        pos_ = 0;
    }

    size_t pos() {return globalPosition_+pos_;}

};

}