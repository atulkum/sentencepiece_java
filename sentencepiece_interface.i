%module sentencepiece_interface

%include "std_string.i"
%{
#include<string>
class SentencePieceInterface;
extern SentencePieceInterface *get_model(const std::string& model_file);
extern std::string encode_pieces(SentencePieceInterface *sp, const std::string& str);
extern std::string encode_ids(SentencePieceInterface *sp, const std::string& str);
extern void unload_model(SentencePieceInterface *sp);
%}
%newobject get_model;
extern SentencePieceInterface *get_model(const std::string& model_file);
extern std::string encode_pieces(SentencePieceInterface *sp, const std::string& str);
extern std::string encode_ids(SentencePieceInterface *sp, const std::string& str);
extern void unload_model(SentencePieceInterface *sp);
