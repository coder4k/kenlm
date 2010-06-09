#include "lm/read_vocab.hh"

#include <istream>
#include <iostream>

#include <ctype.h>
#include <err.h>

namespace lm {

void ReadSingleVocab(std::istream &in, boost::unordered_set<std::string> &out) {
  in.exceptions(std::istream::badbit);
  std::string word;
  while (in >> word) {
    out.insert(word);
  }
}

namespace {
bool IsLineEnd(std::istream &in) {
  int got;
  do {
    got = in.get();
    if (!in) return true;
    if (got == '\n') return true;
  } while (isspace(got));
  in.unget();
  return false;
}
}// namespace

// Read space separated words in enter separated lines.  These lines can be
// very long, so don't read an entire line at a time.  
unsigned int ReadMultipleVocab(std::istream &in, boost::unordered_map<std::string, std::vector<unsigned int> > &out) {
  in.exceptions(std::istream::badbit);
  unsigned int sentence = 0;
  bool used_id = false;
  std::string word;
  while (in >> word) {
    used_id = true;
    std::vector<unsigned int> &posting = out[word];
    if (posting.empty() || (posting.back() != sentence))
      posting.push_back(sentence);
    if (IsLineEnd(in)) {
      ++sentence;
      used_id = false;
    }
  }
  return sentence + used_id;
}

} // namespace lm
