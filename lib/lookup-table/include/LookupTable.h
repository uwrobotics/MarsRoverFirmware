/**
 * This is a non-mutable data-type based off a std::unordered_map
 * See test-lookup-table for usage
 */

#pragma once
#include <iterator>
#include <optional>
#include <unordered_map>

namespace lookup_table {
namespace internal {
struct UnusedDefaultValue {};
}  // namespace internal
template <typename Key, typename Value, const auto defaultValue = internal::UnusedDefaultValue(),
          typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>,
          typename Allocator = std::allocator<std::pair<const Key, Value>>>
class LookupTable {
 private:
  const std::unordered_map<Key, Value, Hash, KeyEqual, Allocator> m_map;

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
  using const_iterator  = typename std::unordered_map<Key, Value, Hash, KeyEqual, Allocator>::const_iterator;

  LookupTable() = delete;

  // range based constructor
  template <typename InputIterator>
  LookupTable(InputIterator first, InputIterator last) : m_map(first, last) {}
  // copy constructor and assignment
  LookupTable(const LookupTable &copy) = default;
  LookupTable &operator=(const LookupTable &) = default;
  // move constructor and assignment
  LookupTable(LookupTable &&move) = default;
  LookupTable &operator=(LookupTable &&) = default;
  // initializer list constructor
  LookupTable(std::initializer_list<value_type> init, size_type bucket_count = 0, const Hash &hash = Hash(),
              const key_equal &equal = key_equal(), const Allocator &alloc = Allocator())
      : m_map(init, bucket_count, hash, equal, alloc) {}

  const_iterator begin() const {
    return m_map.begin();
  }

  const_iterator end() const {
    return m_map.end();
  }

  std::size_t size() const {
    return m_map.size();
  }
  std::size_t max_size() const {
    return m_map.max_size();
  }

  bool empty() const {
    return m_map.empty();
  }

  template <typename V = Value>
  typename std::enable_if<std::is_same<V, decltype(defaultValue)>::value, V>::type at(const Key &key) const {
    bool value_exists = m_map.find(key) != m_map.end();
    return value_exists ? m_map.at(key) : defaultValue;
  }
  template <typename V = Value>
  typename std::enable_if<!std::is_same<V, decltype(defaultValue)>::value, std::optional<V>>::type at(
      const Key &key) const {
    bool value_exists = m_map.find(key) != m_map.end();
    return value_exists ? std::optional<Value>{m_map.at(key)} : std::nullopt;
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
}  // namespace lookup_table
