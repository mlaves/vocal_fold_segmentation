TEMPLATE = subdirs

# Needed to ensure that things are built right, which you have to do yourself :(
CONFIG += ordered

SUBDIRS += \
    qimagecl \
    mainwindow

# Use .depends to specify that a project depends on another.
mainwindow.depends = qimagecl
