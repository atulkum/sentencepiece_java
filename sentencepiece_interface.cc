#include <string>
#include <vector>

#include <mutex>
#include <string>
#include <unordered_map>
#include <sstream>

#include <sentencepiece_processor.h>

#include <iostream>

class SentencePieceProcessorCache {
 public:
  std::shared_ptr<sentencepiece::SentencePieceProcessor> get(const std::string key) {
    std::lock_guard<std::mutex> l(mutex_);
    const std::size_t file_hash = std::hash<std::string>{}(key);
    auto sp = data_[file_hash].lock();

    if (sp) {
      return sp;
    }

    sp = std::make_shared<sentencepiece::SentencePieceProcessor>();
    auto status = sp->Load(key);
    if (!status.ok()) return nullptr;

    data_[file_hash] = sp;
    return sp;
  }

 private:
  std::mutex mutex_;
  std::unordered_map<std::size_t, std::weak_ptr<sentencepiece::SentencePieceProcessor>> data_;
};

class SentencePieceInterface {
  public:
    explicit SentencePieceInterface(const std::string& model_file) {
      static SentencePieceProcessorCache* cache = new SentencePieceProcessorCache;
      sentencepiece_processor_ = cache->get(model_file);
    }
    std::vector<std::string> encode_pieces(const std::string& str ) {
      return sentencepiece_processor_->EncodeAsPieces(str);
    }
    std::vector<int> encode_ids(const std::string& str) {
      return sentencepiece_processor_->EncodeAsIds(str);
    }
    ~SentencePieceInterface(){
        std::cout << "SentencePieceInterface resource released." << std::endl;
    }
  private:
    std::shared_ptr<sentencepiece::SentencePieceProcessor> sentencepiece_processor_;
};

SentencePieceInterface* get_model(const std::string& model_file){
  return new SentencePieceInterface(model_file);
}
std::string encode_pieces(SentencePieceInterface *sp, const std::string& str){
  std::vector<std::string> result = sp->encode_pieces(str);
  std::stringstream res;
  copy(result.begin(), result.end(), std::ostream_iterator<std::string>(res, " "));
  return res.str();
}
std::string encode_ids(SentencePieceInterface *sp, const std::string& str){
  std::vector<int> result = sp->encode_ids(str);
  std::stringstream res;
  copy(result.begin(), result.end(), std::ostream_iterator<int>(res, " "));
  return res.str();
}
void unload_model(SentencePieceInterface *sp) {
  delete sp;
}

