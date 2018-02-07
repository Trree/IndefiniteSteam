#include "NamedType.hpp"
#include <functional>
#include <iostream>
#include <vector>

namespace indefinite {

template <class T = int> class IndefiniteStream {

public:
  using filter_handler = std::vector<std::function<bool(const T &)>>;
  using scale_handler = std::vector<std::function<T(const T &)>>;
  using Start = NamedType<int, struct StartParameter>;
  using Step = NamedType<int, struct StepParameter>;
  using End = NamedType<int, struct EndParameter>;

  IndefiniteStream() {}
  IndefiniteStream(Start start) : start_(start) {}
  IndefiniteStream(Start start, filter_handler filter)
      : start_(start), filters_(filter) {}
  IndefiniteStream(Start start, Step step, filter_handler filter)
      : start_(start), step_(step), filters_(filter) {}
  IndefiniteStream(Start start, Step step) : start_(start), step_(step) {}
  IndefiniteStream(Start start, Step step, End end)
      : start_(start), step_(step), end_(end) {}
  IndefiniteStream(Start start, Step step, End end, filter_handler filter)
      : start_(start), step_(step), end_(end), filters_(filter) {}
  ~IndefiniteStream() {}

  IndefiniteStream &from(Start start) { start_ = start; }
  IndefiniteStream &from(Start start, Step step) {
    start_ = start;
    step_ = step;
  }

  IndefiniteStream &range(Start start, End end) {
    start_ = start;
    end_ = end;
  }

  IndefiniteStream &filter(std::function<bool(const T &)> f) {
    filters_.push_back(f);
    return *this;
  }
  IndefiniteStream &scale_before(std::function<T(const T &)> f) {
    scales_before_.push_back(f);
    return *this;
  }
  IndefiniteStream &scale_after(std::function<T(const T &)> f) {
    scales_after_.push_back(f);
    return *this;
  }


  T handler_filer(T value) {
    auto it = filters_.begin();
    for (; it != filters_.end(); it++) {
      if (!(*it)(value)) {
        return -1;
      }
    }
    if (it == filters_.end()) {
      return value;
    }
    return -1;
  }

  T handler_scale_before(T value) {
    for (auto scale : scales_before_) {
      value = scale(value);
    }
    return value;
  }
  T handler_scale_after(T value) {
    for (auto scale : scales_after_) {
      value = scale(value);
    }
    return value;
  }

  T top() {
    if (end_.get() == -1 || start_.get() < end_.get()) {
      for (auto i = start_.get();; i += step_.get()) {
        auto value = handler_scale_before(i);
        value = handler_filer(value);
        if (value != -1) {
          start_.get() = i;
          return handler_scale_after(value);
        }
      }
    }
    return -1;
  }

  T pop() {
    if (end_.get() == -1 || start_.get() < end_.get()) {
      for (auto i = start_.get();; i += step_.get()) {
        auto value = handler_scale_before(i);
        value = handler_filer(value);
        if (value != -1) {
          start_.get() = i + step_.get();
          return handler_scale_after(value);
        }
      }
    }
    return -1;
  }

  std::vector<T> limit(T size) {
    std::vector<T> tmp;
    tmp.reserve(size);
    auto i = start_.get();
    auto j = 0;
    while ((end_.get() == -1 || (i <= end_.get())) && j < size) {
      auto value = handler_scale_before(i);
      value = handler_filer(value);
      if (value != -1) {
        tmp.push_back(handler_scale_after(value));
        j++;
      }
      i = i + step_.get();
    }
    return tmp;
  }

  void handlerPrint (End end){
    if (end_.get() != -1 && start_.get() <= end_.get()) {
      for (int i = start_.get(); i <= end_.get(); i += step_.get()) {
        auto value = handler_filer(i);
        if (value != -1) {
          std::cout << handler_scale_after(value) << ' ';
        }
      }
      std::cout << '\n';
    } else {
      return;
    }
  }
  
  void printStream() const {
    handlerPrint(end_);
  }

  void printStream(End end) const {
    handlerPrint(end);
  }

private:
  Start start_ = Start(0);
  Step step_ = Step(1);
  End end_ = End(-1);
  filter_handler filters_;
  scale_handler scales_before_;
  scale_handler scales_after_;
};

} // namespace indefinite
