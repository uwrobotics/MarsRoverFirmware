#pragma once
#include <iterator>
#include <optional>
#include <unordered_map>

namespace LookupTableInternal {
  struct UnusedDefaultValue{};
}
// This a LUT type. It is not meant to be mutated after initilization.
template <typename Key, typename Value, const auto defaultValue = LookupTableInternal::UnusedDefaultValue(), 
          typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>,
          typename Allocator = std::allocator<std::pair<const Key, Value>>>
class LookupTable {
 private:
  const std::unordered_map<Key, Value, Hash, KeyEqual, Allocator> _unordered_map;
 public:
  using key_type        = Key;
  using mapped_type     = Value;
  using value_type      = std::pair<const Key, Value>;
  using size_type       = size_t;
  using difference_type = std::ptrdiff_t;
  using hasher          = Hash;
  using key_equal       = KeyEqual;
  using allocator_type  = Allocator;
  using reference       = value_type &;
  using const_reference = const value_type &;
  using pointer         = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer   = typename std::allocator_traits<Allocator>::const_pointer;

  LookupTable() = delete;

  // range based constructor
  template <typename InputIterator>
  LookupTable(InputIterator first, InputIterator last) : _unordered_map(first, last) {}
  // copy constructor and assignment
  LookupTable(const LookupTable &copy) = default;
  LookupTable &operator=(const LookupTable &) = default;
  // move constructor and assignment
  LookupTable(LookupTable &&move) = default;
  LookupTable &operator=(LookupTable &&) = default;
  // initializer list constructor
  LookupTable(std::initializer_list<value_type> init, size_type bucket_count = 0, const Hash &hash = Hash(),
              const key_equal &equal = key_equal(), const Allocator &alloc = Allocator())
      : _unordered_map(init, bucket_count, hash, equal, alloc) {}

  typename std::unordered_map<Key, Value>::iterator begin() const {
    return _unordered_map.begin();
  }
  typename std::unordered_map<Key, Value>::iterator end() const {
    return _unordered_map.end();
  }

  std::size_t size() const {
    return _unordered_map.size();
  }
  std::size_t max_size() const {
    return _unordered_map.max_size();
  }

  bool empty() const {
    return _unordered_map.empty();
  }

  template <typename V = Value>
  typename std::enable_if<std::is_same<V, decltype(defaultValue)>::value, V>::type at(const Key &key) const {
    bool value_exists = _unordered_map.find(key) != _unordered_map.end();
    return value_exists ? _unordered_map.at(key) : defaultValue;
  }
  template <typename V = Value>
  typename std::enable_if<!std::is_same<V, decltype(defaultValue)>::value, std::optional<V>>::type at(
    const Key &key) const {
    bool value_exists = _unordered_map.find(key) != _unordered_map.end();
    return value_exists ? std::optional<Value>{_unordered_map.at(key)} : std::nullopt;
  }

  template <typename V = Value>
  typename std::enable_if<std::is_same<V, decltype(defaultValue)>::value, V>::type operator[](const Key &key) const {
    return at(key);
  }
  template <typename V = Value>
  typename std::enable_if<!std::is_same<V, decltype(defaultValue)>::value, std::optional<V>>::type operator[](
    const Key &key) const {
    return at(key);
  }
};
