#import "template.typ": template
#import "cfg.typ": cfg

#show: body => template(cfg: cfg, body)

#include "body.typ"
