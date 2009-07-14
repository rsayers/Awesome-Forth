#include <iostream>
#include <vector>
#include <sstream>
#include <cctype>
using std::isalnum;
using std::isalpha;
using std::isdigit;
using std::isxdigit;

#define push(n) stack.push_back(n)
#define DEBUG 0
using namespace std;

void dprn(string msg){
  if (DEBUG){
    cout << msg << endl;
  }
}

struct word{
  string name;
  int type;
  void (*fp)(void);
  string worddef;
  // cheating for now, when a compiled word is called, I will interpret it then
};

// stack is the main instruction stack, where our values will live
vector <int> stack;

//words will hold just that, both builtin and compiled
vector <word> words;

// the tokenized program
vector <string> tokens;



//shamelessly stolen from the C++ HOWTO
void tokenize(const string& str,const string& delimiters = " ")
{
  // Skip delimiters at beginning.
  string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  // Find first "non-delimiter".
  string::size_type pos     = str.find_first_of(delimiters, lastPos);

  while (string::npos != pos || string::npos != lastPos)
    {
      // Found a token, add it to the vector.
      tokens.push_back(str.substr(lastPos, pos - lastPos));
      // Skip delimiters.  Note the "not_of"
      lastPos = str.find_first_not_of(delimiters, pos);
      // Find next "non-delimiter"
      pos = str.find_first_of(delimiters, lastPos);
    }
}

void viewstack(){
  int i;
  
  for (i=0; i<stack.size(); i++){
    cout << i << ": " << stack[i] << "\n";
  }
}

int pop(){
  int n;
  n=stack.back();
  stack.pop_back();
  return n;
}

// Begin primitives

void makeword(string name, int type, void(*fp)()){
  word newword;
  newword.name=name;
  newword.type=type;
  newword.fp=fp;
  words.push_back(newword);
}

void makeword(string name, int type, string body){
  word newword;
  newword.name=name;
  newword.type=type;
  newword.worddef=body;
  words.push_back(newword);
}

void dup(void){
  int n;
  n=pop();
  push(n);
  push(n);
}

void swap(){
  int i,j;
  i=pop();
  j=pop();
  push(i);
  push(j);
}

void add(){
  push(pop()+pop());
}

void sub(){
  int n=pop();
  push(n-pop());
}

void mult(){
  push(pop()*pop());
}

void div(){
  int n=pop();
  push(n/pop());
}

void modulo(){
  push(pop()%pop());
}

void output(){
  cout << pop();
}

void doword(string name){
  int i;

  for(i=0;i<words.size();i++){
    if (words[i].name==name){

      dprn("Found Word: " + name);
      if(words[i].type==1){
	(*words[i].fp)();
      }else{
	tokenize(words[i].worddef);
      }
    }
  }
}



void interpret(){
  int i=0;
  int x;
  string state="parse";
  string wordname="";
  string wordbody="";
    
  for(i=0;i<tokens.size();i++){
    dprn("Token: "+tokens[i]);
    if (tokens[i]==":"){ state="compile"; }
    if (tokens[i]==";"){
      dprn("Adding word: "+wordname);
      dprn("Wordbody: "+wordbody);
      makeword(wordname,2,wordbody);
      wordname="";
      wordbody="";
      state="parse";
    }
    if (state=="compile"){
      if(tokens[i]!=":"){
	if (wordname==""){
	  wordname=tokens[i];
	}else{
	  wordbody = wordbody + " " + tokens[i];
	}
      }
    }else{
      
      istringstream inpStream(tokens[i]);
      int inpValue=0;
      if ( inpStream >> inpValue ){
	push(inpValue);
      }else{
	doword(tokens[i]);
      }
    }
  }
  if (state!="compile"){tokens.clear();}
}
void repl(){
  string in;
  
  while (in!="quit"){
    cout << endl << "forth> ";
    cin >> in;
    tokenize(in);
    interpret();
  }
}

main(){ 

  cout << "AwesomeForth -1.0" << endl;
  cout << "-----------------" << endl;

  makeword("swap",1,&swap);
  makeword("dup",1,&dup);
  makeword("+",1,&add);
  makeword("-",1,&sub);
  makeword("*",1,&mult);
  makeword("/",1,&div);
  makeword("%",1,&modulo);
  makeword(".",1,&output);
 
  //repl();

  interpret();
  repl();
  

}
