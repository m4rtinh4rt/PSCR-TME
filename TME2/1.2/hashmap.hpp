#include <cstddef>
#include <forward_list>
#include <iostream>
#include <vector>

namespace pr {
template <typename K, typename V>
class HashTable {
  class Entry {
   public:
    const K key;
    V value;

    Entry(K key, V value) : key(key), value(value) {}
  };
  typedef std::vector<std::forward_list<Entry>> bucket_t;
  bucket_t buckets;

 public:
  HashTable(size_t init = 100) {
    buckets.reserve(init);
    for (size_t i = 0; i < init; ++i)
      buckets.push_back(std::forward_list<Entry>());
  }

  V* get(const K& key) {
    size_t h = std::hash<K>()(key);
    std::forward_list<Entry>& l = buckets[h % buckets.size()];

    for (Entry& x : l)
      if (x.key == key) return &x.value;
    return nullptr;
  }

  bool put(const K& key, const V& value) {
    size_t size = this->size();
    if ((size * 100 / buckets.size()) >= 80) this->grow();

    size_t h = std::hash<K>()(key);
    std::forward_list<Entry>& l = buckets[h % buckets.size()];

    for (Entry& x : l) {
      if (x.key == key) {
        x.value = value;
        return true;
      }
    }
    l.push_front(Entry(key, value));
    return false;
  }

  size_t size() {
    size_t sum = 0;
    for (std::forward_list<Entry>& l : buckets) {
      sum += std::distance(l.begin(), l.end());
    }
    return sum;
  }

  void grow() {
    size_t len = buckets.size();
    std::vector<Entry> vec;
    vec.reserve(len);

    for (std::forward_list<Entry>& l : buckets) {
      for (Entry& e : l) vec.push_back(e);
    }

    len *= 2;
    buckets.clear();
    buckets.reserve(len);
    for (size_t i = 0; i < len; ++i)
      buckets.push_back(std::forward_list<Entry>());

    for (Entry& e : vec) this->put(e.key, e.value);
  }
};
}  // namespace pr
