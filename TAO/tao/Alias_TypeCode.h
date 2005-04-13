// -*- C++ -*-

//=============================================================================
/**
 *  @file    Alias_TypeCode.h
 *
 *  $Id$
 *
 *  Header file for a @c tk_alias and @c tk_value_box
 *  @c CORBA::TypeCode.
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_ALIAS_TYPECODE_H
#define TAO_ALIAS_TYPECODE_H

#include /**/ "ace/pre.h"

#include "tao/TypeCode.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TypeCodeFactory_Adapter.h"
#include "tao/TypeCode_Base_Attributes.h"


namespace TAO
{
  namespace TypeCode
  {
    template <CORBA::TCKind KIND> struct Alias_Traits;

    template<>
    struct Alias_Traits<CORBA::tk_alias>
    {
      static
      CORBA::TypeCode_ptr
      create_compact_typecode (TAO_TypeCodeFactory_Adapter * factory,
                               char const * id,
                               CORBA::TypeCode_ptr compact_content_type
                               ACE_ENV_ARG_DECL)
      {
        return factory->create_alias_tc (id,
                                         "",  /* empty name */
                                         compact_content_type
                                         ACE_ENV_ARG_PARAMETER);
      }
    };

    template<>
    struct Alias_Traits<CORBA::tk_value_box>
    {
      static
      CORBA::TypeCode_ptr
      create_compact_typecode (TAO_TypeCodeFactory_Adapter * factory,
                               char const * id,
                               CORBA::TypeCode_ptr compact_content_type
                               ACE_ENV_ARG_DECL)
      {
        return factory->create_value_box_tc (id,
                                             "",  /* empty name */
                                             compact_content_type
                                             ACE_ENV_ARG_PARAMETER);
      }
    };



    /**
     * @class Alias
     *
     * @brief @c CORBA::TypeCode implementation for an OMG IDL
     *        @c typedef.
     *
     * This class implements a @c CORBA::TypeCode for an OMG IDL
     * @c typedef.
     */
    template <typename StringType,
              typename TypeCodeType,
              CORBA::TCKind Kind,
              class RefCountPolicy>
    class Alias
      : public CORBA::TypeCode,
        private RefCountPolicy
    {
    public:

      /// Constructor.
      Alias (char const * id,
             char const * name,
             TypeCodeType const & tc);

      /**
       * @name TAO-specific @c CORBA::TypeCode Methods
       *
       * Methods required by TAO's implementation of the
       * @c CORBA::TypeCode class.
       *
       * @see @c CORBA::TypeCode
       */
      //@{
      virtual bool tao_marshal (TAO_OutputCDR & cdr) const;
      virtual void tao_duplicate (void);
      virtual void tao_release (void);
      //@}

    protected:

      /**
       * @name @c TAO CORBA::TypeCode Template Methods
       *
       * @c tk_alias and @c tk_value_box @c CORBA::TypeCode -specific
       * template methods.
       *
       * @see @c CORBA::TypeCode
       */
      //@{
      virtual CORBA::Boolean equal_i (CORBA::TypeCode_ptr tc
                                      ACE_ENV_ARG_DECL) const;
      virtual CORBA::Boolean equivalent_i (CORBA::TypeCode_ptr tc
                                           ACE_ENV_ARG_DECL) const;
      virtual CORBA::TypeCode_ptr get_compact_typecode_i (
        ACE_ENV_SINGLE_ARG_DECL) const;
      virtual char const * id_i (ACE_ENV_SINGLE_ARG_DECL) const;
      virtual char const * name_i (ACE_ENV_SINGLE_ARG_DECL) const;
      virtual CORBA::TypeCode_ptr content_type_i (ACE_ENV_SINGLE_ARG_DECL) const;

    private:

      /// Base attributes for this @c TypeCode containing the
      /// repository ID and name of the @c typedef.
      Base_Attributes<StringType> attributes_;

      /// The @c TypeCode corresponding to the original type upon
      /// which the IDL @c typedef was made.
      /**
       * A pointer to the @c CORBA::TypeCode_ptr rather than the
       * @c CORBA::TypeCode_ptr itself is stored since that address is
       * well-defined.  We may not know the value of the @c
       * CORBA::TypeCode_ptr when creating this @c Field statically at
       * compile-time, hence the indirection.
       *
       * @note This @c TypeCode is released upon destruction of this
       *       @c TypeCode::Alias.
       */
      TypeCodeType const content_type_;

    };

  }  // End namespace TypeCode
}  // End namespace TAO


#ifdef __ACE_INLINE__
# include "tao/Alias_TypeCode.inl"
#endif  /* __ACE_INLINE__ */

#ifdef ACE_TEMPLATES_REQUIRE_SOURCE
# include "tao/Alias_TypeCode.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#ifdef ACE_TEMPLATES_REQUIRE_PRAGMA
# pragma implementation ("Alias_TypeCode.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_ALIAS_TYPECODE_H */
