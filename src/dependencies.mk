
common=
ast=
parser=
codegen=
passes=
sapphire=codegen parser passes ast common

define make_depen
$(eval $1: $($1))
endef
map = $(foreach a,$(2),$(call $(1),$(a)))
define make_prereqs
$(call map,make_depen,sapphire parser ast common codegen passes)
endef
