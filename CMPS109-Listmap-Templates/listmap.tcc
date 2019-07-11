// $Id: listmap.tcc,v 1.11 2018-01-25 14:19:14-08 - - $
// Anthony Lin (anhlin)
// Mihir Malik (mimalik)

#include "listmap.h"
#include "debug.h"

//
/////////////////////////////////////////////////////////////////
// Operations on listmap::node.
/////////////////////////////////////////////////////////////////
//

//
// listmap::node::node (link*, link*, const value_type&)
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::node::node (node* next, node* prev,
                                     const value_type& value):
            link (next, prev), value (value) {
}

//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//

//
// listmap::~listmap()
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::~listmap() {
   DEBUGF ('l', reinterpret_cast<const void*> (this));
}

//
// iterator listmap::insert (const value_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::insert (const value_type& pair) {
   DEBUGF ('l', &pair << "->" << pair);
   iterator it; 
   if(this->empty()){
     node* firstnode; 
     firstnode = new node(anchor(), anchor(), pair);
     anchor()->next = firstnode; 
     anchor()->prev = firstnode;
     it = this->begin(); 
     return it; 
   }
   it = this->find(pair.first); 
   if(it == this->end()){ 
     it = this->begin();
     for(; it != this->end(); ++it){
       if (not less(it->first, pair.first)) break;
     }
     node* newnode = new node(it.where, it.where->prev, pair);
     it.where->prev->next = newnode; 
     it.where->prev = newnode; 
     it = newnode;
     return it;
   }else{
     it->second = pair.second; 
     return it;
   }
   return it;
}

//
// listmap::find(const key_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::find (const key_type& that) {
   DEBUGF ('l', that);
   iterator it = this->begin();
   for(; it != this->end(); ++it){
     if(it->first == that) break;
   }
   return it;
}

//
// iterator listmap::erase (iterator position)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::erase (iterator position) {
   DEBUGF ('l', &*position);
   iterator it = position.where->next;
   position.where->prev->next = position.where->next;
   position.where->next->prev = position.where->prev; 
   position.where->next= nullptr;
   position.where->prev = nullptr; 
   delete position.where;
   return it;
}

///////////////////
// Helper functions
///////////////////
template <typename Key, typename Value, class Less>
void listmap<Key, Value, Less>::print_key(const key_type& that){
  iterator it = this->find(that);
  if(it == this->end()){
    cout << that << ": key not found" << endl;
  }else{
  cout << it->first << " = " << it->second << endl;
  }
}

template <typename Key, typename Value, class Less>
void listmap<Key,Value,Less>::print_map(){
  iterator it = this->begin(); 
  while(it != this->end()){
    cout << it->first << " = " << it->second << endl; 
    ++it;
  }
}

template <typename Key, typename Value, class Less>
void listmap<Key, Value, Less>::delete_key(const key_type& that){
  iterator it = this->find(that);
  if(it == this->end()){
    cout << that << ": key not found" << endl;
  }else{
    erase(it);
  }
}

template <typename Key, typename Value, class Less>
void listmap<Key, Value, Less>::print_value(const mapped_type& that){
  iterator it = this->begin();
  while(it != this->end()){
    if(it->second == that){
      cout << it->first << " = " << it->second << endl;
    }
    ++it;
  }
}

//
/////////////////////////////////////////////////////////////////
// Operations on listmap::iterator.
/////////////////////////////////////////////////////////////////
//

//
// listmap::value_type& listmap::iterator::operator*()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type&
listmap<Key,Value,Less>::iterator::operator*() {
   DEBUGF ('l', where);
   return where->value;
}

//
// listmap::value_type* listmap::iterator::operator->()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type*
listmap<Key,Value,Less>::iterator::operator->() {
   DEBUGF ('l', where);
   return &(where->value);
}

//
// listmap::iterator& listmap::iterator::operator++()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator++() {
   DEBUGF ('l', where);
   where = where->next;
   return *this;
}

//
// listmap::iterator& listmap::iterator::operator--()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator--() {
   DEBUGF ('l', where);
   where = where->prev;
   return *this;
}


//
// bool listmap::iterator::operator== (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator==
            (const iterator& that) const {
   return this->where == that.where;
}

//
// bool listmap::iterator::operator!= (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator!=
            (const iterator& that) const {
   return this->where != that.where;
}

