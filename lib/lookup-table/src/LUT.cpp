#include "LUT.h"

LUT::LUT(InputIterator first, InputIterator last) : _unordered_map(first, last) {}

LUT::LUT(std::initializer_list<value_type> init, size_type bucket_count = 0, const Hash& hash = Hash(), const key_equal& equal = key_equal(), const Allocator& alloc = Allocator()) : _unordered_map(init, bucket_count, hash, equal, alloc) {}

typename std::unordered_map<Key, Value>::iterator LUT::begin() {
  return _unordered_map.begin();
}
typename std::unordered_map<Key, Value>::iterator LUT::end() {
  return _unordered_map.end();
}

std::size_t LUT::size() const {
  return _unordered_map.size();
}
std::size_t LUT::max_size() const {
  return _unordered_map.max_size();
}

bool LUT::empty() const {
  return _unordered_map.empty();
}

std::optional<V> LUT::at(const K &key) const {
  if (_unordered_map.find(key) == _unordered_map.end()) return std::nullopt;
  return _unordered_map.at(key);
}

std::optional<V> LUT::operator[](const K &key) const {
  return at(key);
}