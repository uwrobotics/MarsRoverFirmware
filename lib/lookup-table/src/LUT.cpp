#include "LUT.h"

template <typename InputIterator>
LUT::LUT(InputIterator first, InputIterator last) : _unordered_map(first, last) {}

template <typename K, typename V>
LUT::LUT(const std::unordered_map<K, V> &copy) : _unordered_map(copy) {}

template <typename K, typename V>
LUT::LUT(std::unordered_map<K, V> &&move) : _unordered_map(move) {}

template <typename K, typename V>
LUT::LUT(std::initializer_list<std::pair<const K, V>> il) : _unordered_map(il) {}

template <typename K, typename V>
typename std::unordered_map<K, V>::iterator LUT::begin() noexcept {
  return _unordered_map.begin();
}
typename std::unordered_map<K, V>::iterator LUT::end() noexcept {
  return _unordered_map.end();
}

std::size_t LUT::size() const noexcept {
  return _unordered_map.size();
}
std::size_t LUT::max_size() const noexcept {
  return _unordered_map.max_size();
}

bool LUT::empty() const noexcept {
  return _unordered_map.empty();
}

std::optional<V> LUT::at(const K &key) const {
  if (_unordered_map.find(key) == _unordered_map.end()) return std::nullopt;
  return _unordered_map.at(key);
}

std::optional<V> LUT::operator[](const K &key) const {
  return at(key);
}