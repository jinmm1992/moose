#!/usr/bin/python

import os, sys, re

import ParseGetPot, Factory
from MooseObject import MooseObject
from Warehouse import Warehouse

"""
Parser object for reading GetPot formatted files
"""
class Parser:
  def __init__(self, factory, warehouse):
    self.factory = factory
    self.warehouse = warehouse
    self.params_parsed = set()
    self.params_ignored = set()
    self.root = None

  """
  Parse the passed filename filling the warehouse with populated InputParameter objects
  Error codes:
    0x00 - Success
    0x01 - pyGetpot parsing error
    0x02 - Unrecogonized Boolean key/value pair
    0x04 - Missing required parameter
  """
  def parse(self, filename):
    error_code = 0x00

    try:
      self.root = ParseGetPot.readInputFile(filename)
    except ParseGetPot.ParseException, ex:
      print "Parse Error in " + filename + ": " + ex.msg
      return 0x01 # Parse Error

    error_code = self._parseNode(filename, self.root)

    if len(self.params_ignored):
      print 'Warning detected when parsing file "' + os.path.join(os.getcwd(), filename) + '"'
      print '       Ignored Parameter(s): ', self.params_ignored

    return error_code

  def extractParams(self, filename, params, getpot_node):
    error_code = 0x00
    full_name = getpot_node.fullName()

    # Populate all of the parameters of this test node
    # using the GetPotParser.  We'll loop over the parsed node
    # so that we can keep track of ignored parameters as well
    local_parsed = set()
    for key, value in getpot_node.params.iteritems():
      self.params_parsed.add(full_name + '/' + key)
      local_parsed.add(key)
      if key in params:
        if params.type(key) == list:
          params[key] = value.split(' ')
        else:
          if re.match('".*"', value):   # Strip quotes
            params[key] = value[1:-1]
          else:
            # Prevent bool types from being stored as strings.  This can lead to the
            # strange situation where string('False') evaluates to true...
            if params.isValid(key) and (type(params[key]) == type(bool())):
              # We support using the case-insensitive strings {true, false} and the string '0', '1'.
              if (value.lower()=='true') or (value=='1'):
                params[key] = True
              elif (value.lower()=='false') or (value=='0'):
                params[key] = False
              else:
                print "Unrecognized (key,value) pair: (", key, ',', value, ")"
                return 0x02

              # Otherwise, just do normal assignment
            else:
              params[key] = value
      else:
        self.params_ignored.add(key)

    # Make sure that all required parameters are supplied
    required_params_missing = params.required_keys() - local_parsed
    if len(required_params_missing):
      print 'Error detected when parsing file "' + os.path.join(os.getcwd(), filename) + '"'
      print '       Required Missing Parameter(s): ', required_params_missing
      error_code = 0x04 # Missing required params

    return error_code

  # private:
  def _parseNode(self, filename, node):
    error_code = 0x00

    if 'type' in node.params:
      moose_type = node.params['type']

      # Get the valid Params for this type
      params = self.factory.validParams(moose_type)

      # Extract the parameters from the Getpot node
      error_code = error_code | self.extractParams(filename, params, node)

      # Add factory and warehouse as private params of the object
      params.addPrivateParam('_factory', self.factory)
      params.addPrivateParam('_warehouse', self.warehouse)
      params.addPrivateParam('_parser', self)
      params.addPrivateParam('_root', self.root)

      # Build the object
      moose_object = self.factory.create(moose_type, node.name, params)

      # Put it in the warehouse
      self.warehouse.addObject(moose_object)

    # Loop over the section names and parse them
    for child in node.children_list:
      error_code = error_code | self._parseNode(filename, node.children[child])

    return error_code
