# Author: Donald Cheung <jianzhang9102@gmail.com>

import os
import re
import sys

def check_float(float_str):
    """ Judge whether a string is a float number

        Parameters
        ----------
        float_str : str
            The string to check

        Returns
        -------
        isfloat : boolean
            True if float_str is a float number, otherwise False
    """
    float_pattern = re.compile("^[-+]?[0-9]+(?:\.[0-9]+)?(?:[eE][-+]?[0-9]+)?$")
    if not re.match(float_pattern, float_str):
        return False
    return True

def _color_code(color, color_type):
    """ Get a color's shell code

        Parameters
        ----------
        color : string
            Color to be coded

            If color_type is "font_color", it should be one of the following colors:
                ===================================================
                "black", "red", "green", "brown", "blue", "purple",
                "cyan", "white", "open_underline", "close_underline"
                ===================================================

            If color_type is "bg_color", it should be one of the following colors:
                =================================
                "black", "red", "green", "brown",
                "blue", "purple", "cyan", "white"
                =================================

        color_type : string                    
            Color's type, should be "font_color" or "bg_color"

        Returns
        -------
        color_code : int
            Color's shell code
    """
    color_type_dict = {"font_color" : dict(),
                       "bg_color" : dict()}

    color_type_dict["font_color"] = {"black"  : 30, 
                                     "red"    : 31,
                                     "green"  : 32,
                                     "brown"  : 33,
                                     "blue"   : 34,
                                     "purple" : 35,
                                     "cyan"   : 36,
                                     "white"  : 37,
                                     "open_underline"  : 38,
                                     "close_underline" : 39}
    
    color_type_dict["bg_color"] = {"black"  : 40,
                                   "red"    : 41,
                                   "green"  : 42,
                                   "brown"  : 43,
                                   "blue"   : 44,
                                   "purple" : 45,
                                   "cyan"   : 46,
                                   "white"  : 47}

    if color_type not in color_type_dict:
        raise ValueError('Invalid color_type %s' % color_type)

    if color not in color_type_dict[color_type]:
        raise ValueError('Invalid color %s for color_type %s' % (color, color_type))

    return color_type_dict[color_type][color]

def color_str(info, font_color="red", bg_color=None, highlight=True):
    """ Color a string for shell to use

        Parameters
        ----------
        info : string
            The string to color

        font_color : string
            String's font color, should be one of the following colors:
                ===================================================
                "black", "red", "green", "brown", "blue", "purple",
                "cyan", "white", "open_underline", "close_underline"
                ===================================================
    
        bg_color : string                    
            String's background color, should be one of the following colors:
                =================================
                "black", "red", "green", "brown",
                "blue", "purple", "cyan", "white"
                =================================

        Returns
        -------
        color_str : string
            String contains shell's color info
    """
    color_list = []
    if highlight:
        color_list.append(str(1))

    if font_color:
        color_list.append(str(_color_code(font_color, "font_color")))

    if bg_color:
        color_list.append(str(_color_code(bg_color, "bg_color")))

    ret_str = "\033[%sm" % (";".join(color_list))
    ret_str += info
    ret_str += "\033[0m"
    return ret_str

def shell_info(info):
    """ Write a shell info str to stderr

        Parameters
        ----------
        info: str
            The information str to print
    """
    sys.stderr.write(color_str(info, "red", "black"))

