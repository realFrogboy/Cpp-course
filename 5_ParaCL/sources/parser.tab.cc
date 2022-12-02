// A Bison parser, made by GNU Bison 3.5.1.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2020 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.





#include "parser.tab.hh"


// Unqualified %code blocks.
#line 19 "parser.y"

#include "driver.hpp"
namespace yy { parser::token_type yylex(parser::semantic_type* yylval, driver_t &driver); }

#line 50 "parser.tab.cc"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif



// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 122 "parser.tab.cc"


  /// Build a parser object.
  parser::parser (yy::driver_t &drv_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      drv (drv_yyarg)
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
  `---------------*/

  // basic_symbol.
#if 201103L <= YY_CPLUSPLUS
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (basic_symbol&& that)
    : Base (std::move (that))
    , value ()
  {
    switch (this->type_get ())
    {
      case 24: // NUMBER
      case 26: // FUNC
        value.move< int > (std::move (that.value));
        break;

      case 25: // NAME
        value.move< name_t* > (std::move (that.value));
        break;

      case 29: // exp
      case 30: // list
      case 31: // stmt
        value.move< node_t* > (std::move (that.value));
        break;

      default:
        break;
    }

  }
#endif

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
  {
    switch (this->type_get ())
    {
      case 24: // NUMBER
      case 26: // FUNC
        value.copy< int > (YY_MOVE (that.value));
        break;

      case 25: // NAME
        value.copy< name_t* > (YY_MOVE (that.value));
        break;

      case 29: // exp
      case 30: // list
      case 31: // stmt
        value.copy< node_t* > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }



  template <typename Base>
  bool
  parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->type_get ())
    {
      case 24: // NUMBER
      case 26: // FUNC
        value.move< int > (YY_MOVE (s.value));
        break;

      case 25: // NAME
        value.move< name_t* > (YY_MOVE (s.value));
        break;

      case 29: // exp
      case 30: // list
      case 31: // stmt
        value.move< node_t* > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

  }

  // by_type.
  parser::by_type::by_type ()
    : type (empty_symbol)
  {}

#if 201103L <= YY_CPLUSPLUS
  parser::by_type::by_type (by_type&& that)
    : type (that.type)
  {
    that.clear ();
  }
#endif

  parser::by_type::by_type (const by_type& that)
    : type (that.type)
  {}

  parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  void
  parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  void
  parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  int
  parser::by_type::type_get () const YY_NOEXCEPT
  {
    return type;
  }


  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_number_type
  parser::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[+state];
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state))
  {
    switch (that.type_get ())
    {
      case 24: // NUMBER
      case 26: // FUNC
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case 25: // NAME
        value.YY_MOVE_OR_COPY< name_t* > (YY_MOVE (that.value));
        break;

      case 29: // exp
      case 30: // list
      case 31: // stmt
        value.YY_MOVE_OR_COPY< node_t* > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s)
  {
    switch (that.type_get ())
    {
      case 24: // NUMBER
      case 26: // FUNC
        value.move< int > (YY_MOVE (that.value));
        break;

      case 25: // NAME
        value.move< name_t* > (YY_MOVE (that.value));
        break;

      case 29: // exp
      case 30: // list
      case 31: // stmt
        value.move< node_t* > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 24: // NUMBER
      case 26: // FUNC
        value.copy< int > (that.value);
        break;

      case 25: // NAME
        value.copy< name_t* > (that.value);
        break;

      case 29: // exp
      case 30: // list
      case 31: // stmt
        value.copy< node_t* > (that.value);
        break;

      default:
        break;
    }

    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 24: // NUMBER
      case 26: // FUNC
        value.move< int > (that.value);
        break;

      case 25: // NAME
        value.move< name_t* > (that.value);
        break;

      case 29: // exp
      case 30: // list
      case 31: // stmt
        value.move< node_t* > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
#if defined __GNUC__ && ! defined __clang__ && ! defined __ICC && __GNUC__ * 100 + __GNUC_MINOR__ <= 408
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
#endif
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " (";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.type = yytranslate_ (yylex (&yyla.value, drv));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case 24: // NUMBER
      case 26: // FUNC
        yylhs.value.emplace< int > ();
        break;

      case 25: // NAME
        yylhs.value.emplace< name_t* > ();
        break;

      case 29: // exp
      case 30: // list
      case 31: // stmt
        yylhs.value.emplace< node_t* > ();
        break;

      default:
        break;
    }



      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2:
#line 68 "parser.y"
                                                  { yylhs.value.as < node_t* > () = new flow_t (flow_type::IF, yystack_[4].value.as < node_t* > (), yystack_[1].value.as < node_t* > (), nullptr); }
#line 705 "parser.tab.cc"
    break;

  case 3:
#line 69 "parser.y"
                                                                              { yylhs.value.as < node_t* > () = new flow_t (flow_type::IF, yystack_[8].value.as < node_t* > (), yystack_[5].value.as < node_t* > (), yystack_[1].value.as < node_t* > ()); }
#line 711 "parser.tab.cc"
    break;

  case 4:
#line 70 "parser.y"
                                                     { yylhs.value.as < node_t* > () = new flow_t (flow_type::WHILE, yystack_[4].value.as < node_t* > (), yystack_[1].value.as < node_t* > (), nullptr); }
#line 717 "parser.tab.cc"
    break;

  case 5:
#line 73 "parser.y"
      { yylhs.value.as < node_t* > () = nullptr; }
#line 723 "parser.tab.cc"
    break;

  case 6:
#line 74 "parser.y"
                {
        if (yystack_[0].value.as < node_t* > () == nullptr)
            yylhs.value.as < node_t* > () = yystack_[1].value.as < node_t* > ();
        else
            yylhs.value.as < node_t* > () = new binop_t{binop_type::SCOLON, yystack_[1].value.as < node_t* > (), yystack_[0].value.as < node_t* > ()};
    }
#line 734 "parser.tab.cc"
    break;

  case 7:
#line 80 "parser.y"
                      {
        if (yystack_[0].value.as < node_t* > () == nullptr)
            yylhs.value.as < node_t* > () = yystack_[2].value.as < node_t* > ();
        else
            yylhs.value.as < node_t* > () = new binop_t{binop_type::SCOLON, yystack_[2].value.as < node_t* > (), yystack_[0].value.as < node_t* > ()};
    }
#line 745 "parser.tab.cc"
    break;

  case 8:
#line 88 "parser.y"
                     { yylhs.value.as < node_t* > () = new binop_t{binop_type::G, yystack_[2].value.as < node_t* > (), yystack_[0].value.as < node_t* > ()}; }
#line 751 "parser.tab.cc"
    break;

  case 9:
#line 89 "parser.y"
                   { yylhs.value.as < node_t* > () = new binop_t{binop_type::L, yystack_[2].value.as < node_t* > (), yystack_[0].value.as < node_t* > ()}; }
#line 757 "parser.tab.cc"
    break;

  case 10:
#line 90 "parser.y"
                    { yylhs.value.as < node_t* > () = new binop_t{binop_type::E, yystack_[2].value.as < node_t* > (), yystack_[0].value.as < node_t* > ()}; }
#line 763 "parser.tab.cc"
    break;

  case 11:
#line 91 "parser.y"
                     { yylhs.value.as < node_t* > () = new binop_t{binop_type::NE, yystack_[2].value.as < node_t* > (), yystack_[0].value.as < node_t* > ()}; }
#line 769 "parser.tab.cc"
    break;

  case 12:
#line 92 "parser.y"
                     { yylhs.value.as < node_t* > () = new binop_t{binop_type::GE, yystack_[2].value.as < node_t* > (), yystack_[0].value.as < node_t* > ()}; }
#line 775 "parser.tab.cc"
    break;

  case 13:
#line 93 "parser.y"
                     { yylhs.value.as < node_t* > () = new binop_t{binop_type::LE, yystack_[2].value.as < node_t* > (), yystack_[0].value.as < node_t* > ()}; }
#line 781 "parser.tab.cc"
    break;

  case 14:
#line 94 "parser.y"
                   { yylhs.value.as < node_t* > () = new binop_t{binop_type::ADD, yystack_[2].value.as < node_t* > (), yystack_[0].value.as < node_t* > ()}; }
#line 787 "parser.tab.cc"
    break;

  case 15:
#line 95 "parser.y"
                    { yylhs.value.as < node_t* > () = new binop_t{binop_type::SUB, yystack_[2].value.as < node_t* > (), yystack_[0].value.as < node_t* > ()};}
#line 793 "parser.tab.cc"
    break;

  case 16:
#line 96 "parser.y"
                   { yylhs.value.as < node_t* > () = new binop_t{binop_type::MUL, yystack_[2].value.as < node_t* > (), yystack_[0].value.as < node_t* > ()}; }
#line 799 "parser.tab.cc"
    break;

  case 17:
#line 97 "parser.y"
                    { yylhs.value.as < node_t* > () = new binop_t{binop_type::DIV, yystack_[2].value.as < node_t* > (), yystack_[0].value.as < node_t* > ()}; }
#line 805 "parser.tab.cc"
    break;

  case 18:
#line 98 "parser.y"
                  { yylhs.value.as < node_t* > () = new binop_t{binop_type::POW, yystack_[2].value.as < node_t* > (), yystack_[0].value.as < node_t* > ()}; }
#line 811 "parser.tab.cc"
    break;

  case 19:
#line 99 "parser.y"
                        { yylhs.value.as < node_t* > () = yystack_[1].value.as < node_t* > (); }
#line 817 "parser.tab.cc"
    break;

  case 20:
#line 100 "parser.y"
                             { yylhs.value.as < node_t* > () = new unop_t{unop_type::MINUS, yystack_[0].value.as < node_t* > ()}; }
#line 823 "parser.tab.cc"
    break;

  case 21:
#line 101 "parser.y"
             { yylhs.value.as < node_t* > () = new num_t{yystack_[0].value.as < int > ()}; }
#line 829 "parser.tab.cc"
    break;

  case 22:
#line 102 "parser.y"
           { yylhs.value.as < node_t* > () = new variable_t(*yystack_[0].value.as < name_t* > ()); }
#line 835 "parser.tab.cc"
    break;

  case 23:
#line 103 "parser.y"
                     { yylhs.value.as < node_t* > () = new binop_t{binop_type::ASSIGN, yystack_[2].value.as < node_t* > (), yystack_[0].value.as < node_t* > ()}; }
#line 841 "parser.tab.cc"
    break;

  case 24:
#line 104 "parser.y"
                             { yylhs.value.as < node_t* > () = new func_t{yystack_[3].value.as < int > (), yystack_[1].value.as < node_t* > ()}; }
#line 847 "parser.tab.cc"
    break;

  case 25:
#line 105 "parser.y"
          { std::cout << "BAD INPUT" << std::endl; }
#line 853 "parser.tab.cc"
    break;

  case 26:
#line 108 "parser.y"
                  {
        tree_t ast{yystack_[1].value.as < node_t* > ()};
        ast.dump();
    }
#line 862 "parser.tab.cc"
    break;


#line 866 "parser.tab.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yysyntax_error_ (yystack_[0].state, yyla));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[+yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yy_error_token_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yy_error_token_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }


      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (state_type, const symbol_type&) const
  {
    return YY_("syntax error");
  }


  const signed char parser::yypact_ninf_ = -12;

  const signed char parser::yytable_ninf_ = -1;

  const signed char
  parser::yypact_[] =
  {
      28,   -12,    36,    36,    -7,     1,   -12,   -12,     2,    59,
      10,    28,    23,   -12,    76,    36,    36,    36,    36,    36,
      36,    36,    36,    36,    28,    36,    36,    36,    36,    36,
      36,   -12,   -12,   -12,   -12,    93,   110,   127,   -12,     0,
      32,    32,    26,    26,   -12,     0,     0,     0,     0,     0,
       0,    20,    21,   -12,    28,    28,    22,    24,    25,   -12,
      33,    28,    35,   -12
  };

  const signed char
  parser::yydefact_[] =
  {
       5,    25,     0,     0,     0,     0,    21,    22,     0,     0,
       0,     5,     0,    20,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     5,     0,     0,     0,     0,     0,
       0,    26,     6,     1,    19,     0,     0,     0,    18,    23,
      15,    14,    16,    17,     7,     8,     9,    10,    11,    12,
      13,     0,     0,    24,     5,     5,     0,     0,     2,     4,
       0,     5,     0,     3
  };

  const signed char
  parser::yypgoto_[] =
  {
     -12,    -1,   -11,   -12,   -12
  };

  const signed char
  parser::yydefgoto_[] =
  {
      -1,     9,    10,    11,    12
  };

  const signed char
  parser::yytable_[] =
  {
      32,    13,    14,    15,    18,    19,    20,    21,    22,    23,
      31,    16,    17,    44,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    33,    45,    46,    47,    48,    49,    50,
      18,     1,    54,    55,     2,    58,    18,    59,     3,     1,
      22,    23,     2,    56,    57,    61,     3,    60,    63,     4,
      62,     5,     6,     7,     8,     0,     0,     0,     0,     0,
       6,     7,     8,    18,    19,    20,    21,    22,    23,     0,
       0,     0,     0,    24,    25,    26,    27,    28,    29,    30,
      18,    19,    20,    21,    22,    23,     0,    34,     0,     0,
       0,    25,    26,    27,    28,    29,    30,    18,    19,    20,
      21,    22,    23,     0,    51,     0,     0,     0,    25,    26,
      27,    28,    29,    30,    18,    19,    20,    21,    22,    23,
       0,    52,     0,     0,     0,    25,    26,    27,    28,    29,
      30,    18,    19,    20,    21,    22,    23,     0,    53,     0,
       0,     0,    25,    26,    27,    28,    29,    30
  };

  const signed char
  parser::yycheck_[] =
  {
      11,     2,     3,    10,     4,     5,     6,     7,     8,     9,
       0,    10,    10,    24,    15,    16,    17,    18,    19,    20,
      21,    22,    23,     0,    25,    26,    27,    28,    29,    30,
       4,     3,    12,    12,     6,    13,     4,    13,    10,     3,
       8,     9,     6,    54,    55,    12,    10,    22,    13,    21,
      61,    23,    24,    25,    26,    -1,    -1,    -1,    -1,    -1,
      24,    25,    26,     4,     5,     6,     7,     8,     9,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    19,    20,
       4,     5,     6,     7,     8,     9,    -1,    11,    -1,    -1,
      -1,    15,    16,    17,    18,    19,    20,     4,     5,     6,
       7,     8,     9,    -1,    11,    -1,    -1,    -1,    15,    16,
      17,    18,    19,    20,     4,     5,     6,     7,     8,     9,
      -1,    11,    -1,    -1,    -1,    15,    16,    17,    18,    19,
      20,     4,     5,     6,     7,     8,     9,    -1,    11,    -1,
      -1,    -1,    15,    16,    17,    18,    19,    20
  };

  const signed char
  parser::yystos_[] =
  {
       0,     3,     6,    10,    21,    23,    24,    25,    26,    29,
      30,    31,    32,    29,    29,    10,    10,    10,     4,     5,
       6,     7,     8,     9,    14,    15,    16,    17,    18,    19,
      20,     0,    30,     0,    11,    29,    29,    29,    29,    29,
      29,    29,    29,    29,    30,    29,    29,    29,    29,    29,
      29,    11,    11,    11,    12,    12,    30,    30,    13,    13,
      22,    12,    30,    13
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    28,    31,    31,    31,    30,    30,    30,    29,    29,
      29,    29,    29,    29,    29,    29,    29,    29,    29,    29,
      29,    29,    29,    29,    29,    29,    32
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     7,    11,     7,     0,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     1,     1,     3,     4,     1,     2
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "ERR", "\"**\"", "\"=\"",
  "\"-\"", "\"+\"", "\"*\"", "\"/\"", "\"(\"", "\")\"", "\"{\"", "\"}\"",
  "\";\"", "\">\"", "\"<\"", "\"==\"", "\"!=\"", "\">=\"", "\"<=\"",
  "\"if\"", "\"else\"", "\"while\"", "NUMBER", "NAME", "FUNC", "UMINUS",
  "$accept", "exp", "list", "stmt", "program", YY_NULLPTR
  };


  const signed char
  parser::yyrline_[] =
  {
       0,    68,    68,    69,    70,    73,    74,    80,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   108
  };

  // Print the state stack on the debug stream.
  void
  parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  parser::yy_reduce_print_ (int yyrule)
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  parser::token_number_type
  parser::yytranslate_ (int t)
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const token_number_type
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27
    };
    const int user_token_number_max_ = 282;

    if (t <= 0)
      return yyeof_;
    else if (t <= user_token_number_max_)
      return translate_table[t];
    else
      return yy_undef_token_;
  }

} // yy
#line 1253 "parser.tab.cc"

#line 114 "parser.y"


namespace yy {

parser::token_type yylex(parser::semantic_type *yylval, driver_t &drv) {
    return drv.yylex(yylval);
}

void parser::error(const std::string&){}

}