#!/bin/bash
REMOTEPATH=$1
KRB_USERNAME=$2

echo "Copying full data files from $REMOTEPATH"

ssh $KRB_USERNAME@lxplus "ls $REMOTEPATH"