#! /usr/bin/env python3

# A list of C++ examples to run in order to ensure that they remain
# buildable and runnable over time.  Each tuple in the list contains
#
#     (example_name, do_run, do_valgrind_run).
#
# See test.py for more information.
cpp_examples = [
    ("lr-wpan-data", "True", "True"),
    ("lr-wpan-error-distance-plot", "True", "True"),
    ("lr-wpan-error-model-plot", "True", "True"),
    ("lr-wpan-packet-print", "True", "True"),
    ("lr-wpan-phy-test", "True", "True"),
]

# A list of Python examples to run in order to ensure that they remain
# runnable over time.  Each tuple in the list contains
#
#     (example_name, do_run).
#
# See test.py for more information.
python_examples = []
