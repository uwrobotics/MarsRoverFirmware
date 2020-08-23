#include <iterator>
#include <optional>
#include <unordered_map>

// This a LUT type. It is not meant to be mutated after initilization.
template <typename K, typename V>
class LUT {
 private:
  std::unordered_map<K, V> _unordered_map;

 public:
  // range based constructor
  template <typename InputIterator>
  LUT(InputIterator first, InputIterator last);
  // copy constructor
  LUT(const std::unordered_map<K, V> &copy);
  // move constructor
  LUT(std::unordered_map<K, V> &&move);
  // initializer list constructor
  LUT(std::initializer_list<std::pair<const K, V>> il);

  typename std::unordered_map<K, V>::iterator begin() noexcept;
  typename std::unordered_map<K, V>::iterator end() noexcept;

  std::size_t size() const noexcept;
  std::size_t max_size() const noexcept;

  bool empty() const noexcept;

  std::optional<V> at(const K &key) const;
  std::optional<V> operator[](const K &key) const;
};