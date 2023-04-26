import enum


def PREFIX(enable=True):
    return "\033[" if enable else ""


def SUFFIX(enable=True):
    return "m" if enable else ""


def RESET(enable=True):
    return PREFIX(enable) + ("0" if enable else "") + SUFFIX(enable)


kwargs_modifiers = {
    "bold": 1,
    "dim": 2,
    "italic": 3,
    "underline": 4,
    "blink": 5,
    "background_modifier": 7,
    "strikethrough": 9,
    "double_underline": 21,
    "overline": 53,
}

kwargs_transforms = {"bright": lambda x: x + 60, "background": lambda x: x + 10}


def _APPLY_MODIFIERS(color: int, user_string=None, **kwargs):
    mod_color = color
    mod_str = ""
    for key, value in kwargs.items():
        if value and key in kwargs_modifiers:
            mod_str += ";" + str(kwargs_modifiers[key])
        elif value and key in kwargs_transforms:
            mod_color = kwargs_transforms[key](mod_color)
    return f"{mod_color}{';' + str(user_string) if user_string else ''}{mod_str}"


def COLOR(color: int, enable=True, **kwargs):
    if not enable:
        return ""
    assert color >= 30 and color <= 37

    return PREFIX(enable) + _APPLY_MODIFIERS(color, **kwargs) + SUFFIX(enable)


def COLOR256(color: int, enable=True, **kwargs):
    if not enable:
        return ""
    assert color >= 0 and color <= 255

    return (
        PREFIX(enable)
        + _APPLY_MODIFIERS(38, user_string=f"5;{color}", **kwargs)
        + SUFFIX(enable)
    )


def COLORRGB(r: int, g: int, b: int, enable=True, **kwargs):
    if not enable:
        return ""
    return (
        PREFIX(enable)
        + _APPLY_MODIFIERS(38, user_string=f"2;{r};{g};{b}", **kwargs)
        + SUFFIX(enable)
    )


def BLACK(enable=True, **kwargs):
    return COLOR(30, enable, **kwargs)


def RED(enable=True, **kwargs):
    return COLOR(31, enable, **kwargs)


def GREEN(enable=True, **kwargs):
    return COLOR(32, enable, **kwargs)


def YELLOW(enable=True, **kwargs):
    return COLOR(33, enable, **kwargs)


def BLUE(enable=True, **kwargs):
    return COLOR(34, enable, **kwargs)


def PURPLE(enable=True, **kwargs):
    return COLOR(35, enable, **kwargs)


def CYAN(enable=True, **kwargs):
    return COLOR(36, enable, **kwargs)


def WHITE(enable=True, **kwargs):
    return COLOR(37, enable, **kwargs)
