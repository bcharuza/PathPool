#ifndef CUSTOM_ITERATORS_H
#define CUSTOM_ITERATORS_H

template<typename It_t,typename FieldRefT>
struct field_ref_iterator : It_t
{
  field_ref_iterator(It_t it,FieldRefT ref):
    It_t{it},m_ref{ref}
  {}

  auto
  operator *() const
  {
    return (&It_t::operator*())->*m_ref;
  }
private:
  FieldRefT m_ref;
};

template<typename It_t,typename FieldRefT>
field_ref_iterator<It_t,FieldRefT> field_ref(It_t it, FieldRefT ref)
{
  return field_ref_iterator<It_t,FieldRefT>{it,ref};
}

// template<typename It_t,typename FuncT>
// struct morphed_iterator : It_t
// {
//   morphed_iterator(It_t it,FuncT func):
//     It_t{it},m_func{func}
//   {}

//   auto
//   operator *()
//   {
//     return this->m_func(this->It_t::operator*());
//   }
// private:
//   FuncT m_func;
// };

// template<typename It_t,typename FuncT>
// morphed_iterator<It_t,FuncT> morphed(It_t it, FuncT ref)
// {
//   return morphed_iterator<It_t,FuncT>{it,ref};
// }

#endif /* CUSTOM_ITERATORS_H */
