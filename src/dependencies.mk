
ast=
parser=
sapphire=parser ast

define make_depen
$(eval $1: $($1))
endef
map = $(foreach a,$(2),$(call $(1),$(a)))
define make_prereqs
$(call map,make_depen,sapphire parser ast)
endef
