#!/usr/bin/env python3
"""Score wrapper (partwise/timewise) flavor configuration.

Per-flavor knobs that capture hand-written variations between the partwise
and timewise families. Keys are the outer XSD element names.
"""

SCORE_WRAPPER_FLAVOR_CONFIG = {
    "score-partwise": {
        # ScorePartwise.cpp
        "outer_extra_includes": [],
        "outer_loop_uses_end_var": False,
        # PartwiseMeasure (music-data holder)
        "music_data_holder_attrs_jit": True,
        "music_data_holder_debug_throw": True,
        # PartwisePart (set holder)
        "set_holder_clear_repushes_default": True,
        "set_holder_remove_has_size_guard": True,
        "set_holder_post_loop_required": False,
        "set_holder_first_flag_name": "isFirstAdded",
        "set_holder_use_return_macro": True,
        # Loop body style differences (partwise variant).
        "set_holder_loop_uses_element_name_var": False,
        "set_holder_unexpected_order": "message_first",   # message << ...; isSuccess = false;
        "set_holder_unexpected_msg": "encountered_quoted", # "...: encountered an unexpected element '...'"
        "set_holder_begin_deref_parens": False,           # *mySet.begin() vs *(mySet.begin())
        "set_holder_from_x_before_first_check": True,
        "set_holder_blank_after_first_decl": False,
        "set_holder_blank_inside_else": False,
        "set_holder_child_var_source": "xml_name",        # "xml_name" => camel(xml); "class_name" => pascal_to_camel(cls)
    },
    "score-timewise": {
        # ScoreTimewise.cpp
        "outer_extra_includes": [
            "ezxml/XElement.h",
            "ezxml/XElementIterator.h",
        ],
        "outer_loop_uses_end_var": True,
        # TimewisePart (music-data holder)
        "music_data_holder_attrs_jit": False,
        "music_data_holder_debug_throw": False,
        # TimewiseMeasure (set holder)
        "set_holder_clear_repushes_default": False,
        "set_holder_remove_has_size_guard": False,
        "set_holder_post_loop_required": True,
        "set_holder_first_flag_name": "isFirstTimewisePartFound",
        "set_holder_use_return_macro": False,
        # Loop body style differences (timewise variant).
        "set_holder_loop_uses_element_name_var": True,
        "set_holder_unexpected_order": "issuccess_first", # isSuccess = false; message << ...;
        "set_holder_unexpected_msg": "trailing_encountered", # "...: unexpected element '...' encountered"
        "set_holder_begin_deref_parens": True,
        "set_holder_from_x_before_first_check": True,
        "set_holder_blank_after_first_decl": True,
        "set_holder_blank_inside_else": True,
        "set_holder_child_var_source": "class_name",
    },
}
