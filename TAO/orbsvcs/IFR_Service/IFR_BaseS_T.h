// -*- C++ -*-
//
// $Id$

// ****  Code generated by the The ACE ORB (TAO) IDL Compiler ****
// TAO and the TAO IDL Compiler have been developed by:
//       Center for Distributed Object Computing
//       Washington University
//       St. Louis, MO
//       USA
//       http://www.cs.wustl.edu/~schmidt/doc-center.html
// and
//       Distributed Object Computing Laboratory
//       University of California at Irvine
//       Irvine, CA
//       USA
//       http://doc.ece.uci.edu/
//
// Information about TAO is available at:
//     http://www.cs.wustl.edu/~schmidt/TAO.html

#ifndef _TAO_IDL_IFR_BASES_T_H_
#define _TAO_IDL_IFR_BASES_T_H_

#include "ace/pre.h"
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

#if defined (ACE_HAS_USING_KEYWORD)

TAO_NAMESPACE  POA_CORBA
{
}
TAO_NAMESPACE_CLOSE // module CORBA

// TIE class: Refer to CORBA v2.2, Section 20.34.4
template <class T>
class TAO_IFR_Client_Export POA_CORBA_IRObject_tie : public POA_CORBA_IRObject
{
public:
  POA_CORBA_IRObject_tie (T &t);
  // the T& ctor
  POA_CORBA_IRObject_tie (T &t, PortableServer::POA_ptr poa);
  // ctor taking a POA
  POA_CORBA_IRObject_tie (T *tp, CORBA::Boolean release=1);
  // ctor taking pointer and an ownership flag
  POA_CORBA_IRObject_tie (T *tp, PortableServer::POA_ptr poa, CORBA::Boolean release=1);
  // ctor with T*, ownership flag and a POA
  ~POA_CORBA_IRObject_tie (void);
  // dtor
  
  // TIE specific functions
  T *_tied_object (void);
  // return the underlying object
  void _tied_object (T &obj);
  // set the underlying object
  void _tied_object (T *obj, CORBA::Boolean release=1);
  // set the underlying object and the ownership flag
  CORBA::Boolean _is_owner (void);
  // do we own it
  void _is_owner (CORBA::Boolean b);
  // set the ownership
  
  // overridden ServantBase operations
  PortableServer::POA_ptr _default_POA (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    );
  CORBA::DefinitionKind def_kind (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    void destroy (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  private:
  T *ptr_;
  PortableServer::POA_var poa_;
  CORBA::Boolean rel_;
  
  // copy and assignment are not allowed
  POA_CORBA_IRObject_tie (const POA_CORBA_IRObject_tie &);
  void operator= (const POA_CORBA_IRObject_tie &);
};

// TIE class: Refer to CORBA v2.2, Section 20.34.4
template <class T>
class TAO_IFR_Client_Export POA_CORBA_Contained_tie : public POA_CORBA_Contained
{
public:
  POA_CORBA_Contained_tie (T &t);
  // the T& ctor
  POA_CORBA_Contained_tie (T &t, PortableServer::POA_ptr poa);
  // ctor taking a POA
  POA_CORBA_Contained_tie (T *tp, CORBA::Boolean release=1);
  // ctor taking pointer and an ownership flag
  POA_CORBA_Contained_tie (T *tp, PortableServer::POA_ptr poa, CORBA::Boolean release=1);
  // ctor with T*, ownership flag and a POA
  ~POA_CORBA_Contained_tie (void);
  // dtor
  
  // TIE specific functions
  T *_tied_object (void);
  // return the underlying object
  void _tied_object (T &obj);
  // set the underlying object
  void _tied_object (T *obj, CORBA::Boolean release=1);
  // set the underlying object and the ownership flag
  CORBA::Boolean _is_owner (void);
  // do we own it
  void _is_owner (CORBA::Boolean b);
  // set the ownership
  
  // overridden ServantBase operations
  PortableServer::POA_ptr _default_POA (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    );
  char * id (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    void id (
      const char * id,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    char * name (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    void name (
      const char * name,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    char * version (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    void version (
      const char * version,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    CORBA_Container_ptr defined_in (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    char * absolute_name (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    CORBA_Repository_ptr containing_repository (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    CORBA_Contained::Description * describe (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    void move (
      CORBA_Container_ptr new_container,
      const char * new_name,
      const char * new_version,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    CORBA::DefinitionKind def_kind (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    void destroy (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  private:
  T *ptr_;
  PortableServer::POA_var poa_;
  CORBA::Boolean rel_;
  
  // copy and assignment are not allowed
  POA_CORBA_Contained_tie (const POA_CORBA_Contained_tie &);
  void operator= (const POA_CORBA_Contained_tie &);
};

// TIE class: Refer to CORBA v2.2, Section 20.34.4
template <class T>
class TAO_IFR_Client_Export POA_CORBA_Container_tie : public POA_CORBA_Container
{
public:
  POA_CORBA_Container_tie (T &t);
  // the T& ctor
  POA_CORBA_Container_tie (T &t, PortableServer::POA_ptr poa);
  // ctor taking a POA
  POA_CORBA_Container_tie (T *tp, CORBA::Boolean release=1);
  // ctor taking pointer and an ownership flag
  POA_CORBA_Container_tie (T *tp, PortableServer::POA_ptr poa, CORBA::Boolean release=1);
  // ctor with T*, ownership flag and a POA
  ~POA_CORBA_Container_tie (void);
  // dtor
  
  // TIE specific functions
  T *_tied_object (void);
  // return the underlying object
  void _tied_object (T &obj);
  // set the underlying object
  void _tied_object (T *obj, CORBA::Boolean release=1);
  // set the underlying object and the ownership flag
  CORBA::Boolean _is_owner (void);
  // do we own it
  void _is_owner (CORBA::Boolean b);
  // set the ownership
  
  // overridden ServantBase operations
  PortableServer::POA_ptr _default_POA (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    );
  CORBA_Contained_ptr lookup (
      const char * search_name,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    CORBA_ContainedSeq * contents (
      CORBA::DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    CORBA_ContainedSeq * lookup_name (
      const char * search_name,
      CORBA::Long levels_to_search,
      CORBA::DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    CORBA_Container::DescriptionSeq * describe_contents (
      CORBA::DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited,
      CORBA::Long max_returned_objs,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    CORBA_ModuleDef_ptr create_module (
      const char * id,
      const char * name,
      const char * version,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    CORBA_ConstantDef_ptr create_constant (
      const char * id,
      const char * name,
      const char * version,
      CORBA_IDLType_ptr type,
      const CORBA::Any & value,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    CORBA_StructDef_ptr create_struct (
      const char * id,
      const char * name,
      const char * version,
      const CORBA_StructMemberSeq & members,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    CORBA_UnionDef_ptr create_union (
      const char * id,
      const char * name,
      const char * version,
      CORBA_IDLType_ptr discriminator_type,
      const CORBA_UnionMemberSeq & members,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    CORBA_EnumDef_ptr create_enum (
      const char * id,
      const char * name,
      const char * version,
      const CORBA_EnumMemberSeq & members,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    CORBA_AliasDef_ptr create_alias (
      const char * id,
      const char * name,
      const char * version,
      CORBA_IDLType_ptr original_type,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    CORBA_InterfaceDef_ptr create_interface (
      const char * id,
      const char * name,
      const char * version,
      const CORBA_InterfaceDefSeq & base_interfaces,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    CORBA_ValueDef_ptr create_value (
      const char * id,
      const char * name,
      const char * version,
      CORBA::Boolean is_custom,
      CORBA::Boolean is_abstract,
      CORBA_ValueDef_ptr base_value,
      CORBA::Boolean is_truncatable,
      const CORBA_ValueDefSeq & abstract_base_values,
      const CORBA_InterfaceDefSeq & supported_interfaces,
      const CORBA_InitializerSeq & initializers,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    CORBA_ValueBoxDef_ptr create_value_box (
      const char * id,
      const char * name,
      const char * version,
      CORBA_IDLType_ptr original_type_def,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    CORBA_ExceptionDef_ptr create_exception (
      const char * id,
      const char * name,
      const char * version,
      const CORBA_StructMemberSeq & members,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    CORBA_NativeDef_ptr create_native (
      const char * id,
      const char * name,
      const char * version,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    CORBA_AbstractInterfaceDef_ptr create_abstract_interface (
      const char * id,
      const char * name,
      const char * version,
      const CORBA_AbstractInterfaceDefSeq & base_interfaces,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    CORBA_LocalInterfaceDef_ptr create_local_interface (
      const char * id,
      const char * name,
      const char * version,
      const CORBA_InterfaceDefSeq & base_interfaces,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    CORBA::DefinitionKind def_kind (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    void destroy (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  private:
  T *ptr_;
  PortableServer::POA_var poa_;
  CORBA::Boolean rel_;
  
  // copy and assignment are not allowed
  POA_CORBA_Container_tie (const POA_CORBA_Container_tie &);
  void operator= (const POA_CORBA_Container_tie &);
};

// TIE class: Refer to CORBA v2.2, Section 20.34.4
template <class T>
class TAO_IFR_Client_Export POA_CORBA_IDLType_tie : public POA_CORBA_IDLType
{
public:
  POA_CORBA_IDLType_tie (T &t);
  // the T& ctor
  POA_CORBA_IDLType_tie (T &t, PortableServer::POA_ptr poa);
  // ctor taking a POA
  POA_CORBA_IDLType_tie (T *tp, CORBA::Boolean release=1);
  // ctor taking pointer and an ownership flag
  POA_CORBA_IDLType_tie (T *tp, PortableServer::POA_ptr poa, CORBA::Boolean release=1);
  // ctor with T*, ownership flag and a POA
  ~POA_CORBA_IDLType_tie (void);
  // dtor
  
  // TIE specific functions
  T *_tied_object (void);
  // return the underlying object
  void _tied_object (T &obj);
  // set the underlying object
  void _tied_object (T *obj, CORBA::Boolean release=1);
  // set the underlying object and the ownership flag
  CORBA::Boolean _is_owner (void);
  // do we own it
  void _is_owner (CORBA::Boolean b);
  // set the ownership
  
  // overridden ServantBase operations
  PortableServer::POA_ptr _default_POA (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    );
  CORBA::TypeCode_ptr type (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    CORBA::DefinitionKind def_kind (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    void destroy (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  private:
  T *ptr_;
  PortableServer::POA_var poa_;
  CORBA::Boolean rel_;
  
  // copy and assignment are not allowed
  POA_CORBA_IDLType_tie (const POA_CORBA_IDLType_tie &);
  void operator= (const POA_CORBA_IDLType_tie &);
};

// TIE class: Refer to CORBA v2.2, Section 20.34.4
template <class T>
class TAO_IFR_Client_Export POA_CORBA_TypedefDef_tie : public POA_CORBA_TypedefDef
{
public:
  POA_CORBA_TypedefDef_tie (T &t);
  // the T& ctor
  POA_CORBA_TypedefDef_tie (T &t, PortableServer::POA_ptr poa);
  // ctor taking a POA
  POA_CORBA_TypedefDef_tie (T *tp, CORBA::Boolean release=1);
  // ctor taking pointer and an ownership flag
  POA_CORBA_TypedefDef_tie (T *tp, PortableServer::POA_ptr poa, CORBA::Boolean release=1);
  // ctor with T*, ownership flag and a POA
  ~POA_CORBA_TypedefDef_tie (void);
  // dtor
  
  // TIE specific functions
  T *_tied_object (void);
  // return the underlying object
  void _tied_object (T &obj);
  // set the underlying object
  void _tied_object (T *obj, CORBA::Boolean release=1);
  // set the underlying object and the ownership flag
  CORBA::Boolean _is_owner (void);
  // do we own it
  void _is_owner (CORBA::Boolean b);
  // set the ownership
  
  // overridden ServantBase operations
  PortableServer::POA_ptr _default_POA (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    );
  char * id (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    void id (
      const char * id,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    char * name (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    void name (
      const char * name,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    char * version (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    void version (
      const char * version,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    CORBA_Container_ptr defined_in (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    char * absolute_name (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    CORBA_Repository_ptr containing_repository (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    CORBA_Contained::Description * describe (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    void move (
      CORBA_Container_ptr new_container,
      const char * new_name,
      const char * new_version,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    CORBA::TypeCode_ptr type (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    CORBA::DefinitionKind def_kind (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
    void destroy (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  private:
  T *ptr_;
  PortableServer::POA_var poa_;
  CORBA::Boolean rel_;
  
  // copy and assignment are not allowed
  POA_CORBA_TypedefDef_tie (const POA_CORBA_TypedefDef_tie &);
  void operator= (const POA_CORBA_TypedefDef_tie &);
};

#endif /* ACE_HAS_USING_KEYWORD */

#if defined (__ACE_INLINE__)
#include "IFR_BaseS_T.i"
#endif /* defined INLINE */


#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "IFR_BaseS_T.cpp"
#endif /* defined REQUIRED SOURCE */


#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("IFR_BaseS_T.cpp")
#endif /* defined REQUIRED PRAGMA */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* ifndef */
