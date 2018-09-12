/* -*- Mode: C++ -*-
 *   Copyright (C) 2004-2005 Net Integration Technologies, Inc.
 *
 * WvStreams interface for command-line argument processing
 */
#ifndef __WVARGS_H
#define __WVARGS_H

#include "wvstring.h"
#include "wvstringlist.h"
#include "wvtr1.h"

class WvArgsOption;
class WvArgsData;

/*!
  @brief WvArgs - Sane command-line argument processing for WvStreams

  WvArgs allows you to specify a series of typed or callback-enabled
  command-line arguments.  Once all of these arguments are specified,
  the WvArgs::process(..) function can be called to perform the actual
  argument processing.

  Sample usage:

  @code
  #include "wvargs.h"

  static void callback(void *userdata, WvStringParm value)
  {
      wvout->print("callback value = %s, userdata = %s\n",
      value, (const char *)userdata);
  }

  int main(int argc, char **argv)
  {
      WvString str_opt = "default";
      bool bool_opt = false;
      int num_opt = 0;

      WvArgs args;
      args.add_option('s', "str", "Pass a string option", "string", str_opt);
      args.add_set_bool_option('b', "bool", "Set a boolean option", bool_opt);
      args.add_option('n', "num", "Pass a numeric option", "integer", num_opt);
      args.add_option('c', "callback", "Callback option",
		      WvArgs::ArgCallback(callback), (void *)"demo");

      WvStringList remaining_args;
      args.process(argc, argv, &remaining_args);

      wvout->print("str_opt=%s, bool_opt=%s, num_opt=%s\n",
      str_opt, bool_opt, num_opt);
      WvStringList::Iter i(remaining_args);
      for (i.rewind(); i.next(); )
	  wvout->print("rem: %s\n", *i);

      return 0;
  }
  @endcode
  !*/
class WvArgs
{
public:

    //!
    //! The callback type used for switches that do not take a parameter.
    //! It returns true if the switch was parsed correctly.
    //!
    typedef wv::function<bool(void*)> NoArgCallback;
    //!
    //! The callback type used for switches that take a parameter
    //! It returns true if the switch was parsed correctly.
    //!
    typedef wv::function<bool(WvStringParm, void*)> ArgCallback;

private:

    WvArgsData *data;
    WvString args_doc;
    WvString version;
    WvString email;
    WvString header;
    WvString footer;

public:

    WvArgs();
    ~WvArgs();

    //!
    //! Process the command line arguments passed to main() using the
    //! options provided through calls to add_option(..).  If
    //! remaining_args is provided, any remaining arguments after the
    //! command line switches will be appended to this list.
    //!
    bool process(int argc, char **argv,
		 WvStringList *remaining_args = NULL);

    //! Set the --version string
    void set_version(WvStringParm version);

    //! Set the e-mail address for bug reports
    void set_email(WvStringParm email);

    //! Set the introductory help message, printed at the beginning of --help
    void set_help_header(WvStringParm header);

    //! Set the descriptive help message, printed at the end of --help
    void set_help_footer(WvStringParm footer);

    //!
    //! Output the short usage message based on the provided options.
    //! Useful when a bad value is passed as the parameter of a switch.
    //!
    void print_usage(int argc, char **argv);
    //!
    //! Output the long usage message based on the provided options.
    //!
    void print_help(int argc, char **argv);

    //!
    //! Add a boolean option, which, when specified, sets the specified
    //! boolean variable to true.
    //!
    //! \param short_option The single-character version of the switch;
    //!        0 for none
    //! \param long_option The full-word version of the switch;
    //!        NULL for none
    //! \param desc The description of the option; NULL for none
    //!
    //! \param val The boolean variable to set to true when the switch
    //!        is specified
    //!
    void add_set_bool_option(char short_option, WvStringParm long_option,
			     WvStringParm desc, bool &val);
    //!
    //! Add a boolean option, which, when spefied, sets the specified
    //! boolean variable to false.
    //!
    //! \param short_option The single-character version of the switch;
    //!        0 for none
    //! \param long_option The full-word version of the switch;
    //!        NULL for none
    //! \param desc The description of the option; NULL for none
    //!
    //! \param val The boolean variable to set to false when the switch
    //!        is specified
    //!
    void add_reset_bool_option(char short_option, WvStringParm long_option,
			       WvStringParm desc, bool &val);
    //!
    //! Add a boolean option, which, when spefied, changes the value
    //! of the boolean variable from false to true or from true to false.
    //!
    //! \param short_option The single-character version of the switch;
    //!        0 for none
    //! \param long_option The full-word version of the switch;
    //!        NULL for none
    //! \param desc The description of the option; NULL for none
    //!
    //! \param val The boolean variable to change when the switch
    //!        is specified
    //!
    void add_flip_bool_option(char short_option, WvStringParm long_option,
			      WvStringParm desc, bool &val);

    //!
    //! Add a switch that takes an integer argument
    //!
    //! \param short_option The single-character version of the switch;
    //!        0 for none
    //! \param long_option The full-word version of the switch;
    //!        NULL for none
    //! \param desc The description of the option; NULL for none
    //! \param arg_desc The (short) description of the argument;
    //!        NULL for none
    //!
    //! \param val The integer varible that gets the value of the argument
    //!
    void add_option(char short_option, WvStringParm long_option,
		    WvStringParm desc, WvStringParm arg_desc, int &val);
    //!
    //! Add a switch that takes a long argument
    //!
    //! \param short_option The single-character version of the switch;
    //!        0 for none
    //! \param long_option The full-word version of the switch;
    //!        NULL for none
    //! \param desc The description of the option; NULL for none
    //! \param arg_desc The (short) description of the argument;
    //!        NULL for none
    //!
    //! \param val The long varible that gets the value of the argument
    //!
    void add_option(char short_option, WvStringParm long_option,
		    WvStringParm desc, WvStringParm arg_desc, long &val);
    //!
    //! Add a switch that takes a float argument
    //!
    //! \param short_option The single-character version of the switch;
    //!        0 for none
    //! \param long_option The full-word version of the switch;
    //!        NULL for none
    //! \param desc The description of the option; NULL for none
    //! \param arg_desc The (short) description of the argument;
    //!        NULL for none
    //!
    //! \param val The float varible that gets the value of the argument
    //!
    void add_option(char short_option, WvStringParm long_option,
		    WvStringParm desc, WvStringParm arg_desc, float &val);
    //!
    //! Add a switch that takes a double argument
    //!
    //! \param short_option The single-character version of the switch;
    //!        0 for none
    //! \param long_option The full-word version of the switch;
    //!        NULL for none
    //! \param desc The description of the option; NULL for none
    //! \param arg_desc The (short) description of the argument;
    //!        NULL for none
    //!
    //! \param val The double varible that gets the value of the argument
    //!
    void add_option(char short_option, WvStringParm long_option,
		    WvStringParm desc, WvStringParm arg_desc, double &val);
    //!
    //! Add a switch that takes a string argument
    //!
    //! \param short_option The single-character version of the switch;
    //!        0 for none
    //! \param long_option The full-word version of the switch;
    //!        NULL for none
    //! \param desc The description of the option; NULL for none
    //! \param arg_desc The (short) description of the argument;
    //!        NULL for none
    //!
    //! \param val The string varible that gets the value of the argument
    //!
    void add_option(char short_option, WvStringParm long_option,
		    WvStringParm desc, WvStringParm arg_desc, WvString &val);
    //!
    //! Add a switch that takes a string argument; the argument is
    //! appended to a string list.
    //!
    //! \param short_option The single-character version of the switch;
    //!        0 for none
    //! \param long_option The full-word version of the switch;
    //!        NULL for none
    //! \param desc The description of the option; NULL for none
    //! \param arg_desc The (short) description of the argument;
    //!        NULL for none
    //!
    //! \param val The string list to which the argument is appended
    //!
    void add_option(char short_option, WvStringParm long_option,
		    WvStringParm desc, WvStringParm arg_desc,
		    WvStringList &val);
    //!
    //! Add a switch which does not take an argument which invokes a
    //! callback when it is specified.
    //!
    //! \param short_option The single-character version of the switch;
    //!        0 for none
    //! \param long_option The full-word version of the switch;
    //!        NULL for none
    //! \param desc The description of the option; NULL for none
    //!
    //! \param cb The callback function to invoke when the switch is
    //!        encountered
    //! \param ud A generic userdata pointer to pass to the callback
    //!
    void add_option(char short_option, WvStringParm long_option,
		    WvStringParm desc, NoArgCallback cb, void *ud = NULL);
    //!
    //! Add a switch which takes an argument which invokes a
    //! callback when it is specified.
    //!
    //! \param short_option The single-character version of the switch;
    //!        0 for none
    //! \param long_option The full-word version of the switch;
    //!        NULL for none
    //! \param desc The description of the option; NULL for none
    //! \param arg_desc The (short) description of the argument;
    //!        NULL for none
    //!
    //! \param cb The callback function to invoke when the switch is
    //!        encountered
    //! \param ud A generic userdata pointer to pass to the callback
    //!
    void add_option(char short_option, WvStringParm long_option,
		    WvStringParm desc, WvStringParm arg_desc,
		    ArgCallback cb, void *ud = NULL);

    //!
    //! Add a required argument to the list of parameters. WvArgs will
    //! return an error when run if it is not specified.
    //!
    //! \param The description of the parameter
    //!
    void add_required_arg(WvStringParm desc, bool multiple = false);
    //!
    //! Add an optional argument to the list of parameters.
    //!
    //! \param The description of the parameter
    //!
    void add_optional_arg(WvStringParm desc, bool multiple = false);

    //!
    //! Remove an option by specifying its short form.
    //!
    //! \note If an option has both a short and a long form they can only
    //! both be removed with two seperate calls to
    //!
    void remove_option(char short_option);
    //!
    //! Remove an option by specifying its long form.
    //!
    //! \note If an option has both a short and a long form they can only
    //! both be removed with two seperate calls to
    //!
    void remove_option(WvStringParm long_option);

    //!
    //! Remove all options
    //!
    void remove_all_options();
    //!
    //! An alias for remove_all_options()
    //!
    void zap()
    {
	remove_all_options();
    }

    //! These flags control the behaviour of WvArgs.  By default, they are
    //! all set to false.
    enum flags_t
    {
	NO_EXIT_ON_ERRORS,	// Do not exit when an error is encountered
	FLAGS_SIZE		// Number of flags that exist
    };

    //!
    //! Get and set flags.
    //!
    bool get_flag(const flags_t flag) const;
    void set_flag(const flags_t flag, const bool value);

};

#endif // __WVARGS_H
