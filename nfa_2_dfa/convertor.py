#!/bin/python

import os

class nka:
	EPS = 'eps'

	headers = {}
	terms = []
	data = {}

	def __init__(self, fname):
		with open(fname) as ifile:
			lines = [l.strip() for l in ifile.readlines()]
			self.parseHeader(lines[0])
			index = 1
			for line in lines[1:]:
				self.parseLine(line, index)
				index += 1

	def parseHeader(self, header):
		fields = header.split('\t')
		self.headers[0] = 'i'
		self.headers[1] = 'o'
		self.headers[2] = 'name'
		for field in fields:
			if field not in self.headers:
				self.headers[len(self.headers)] = field
			else:
				exit("Duplicate in the headers!")
		for header in self.headers:
			if self.headers[header] not in ['i', 'o', 'name', nka.EPS]:
				self.terms.append(header)

	def parseLine(self, line, index):
		fields = line.split('\t')
		oline = {'o': False, 'i': False}
		
		x = 2;
		if '<' in fields[0]:
			oline['o'] = True
		if '>' in fields[0]:
			oline['i'] = True
		if oline['o'] or oline['i']:
			fields = fields[1:]

		field = fields[0]
		term = self.headers[x]
		oline[term] = field
		x += 1

		fields = fields[1:]
		for field in fields:
			term = self.headers[x]
			oline[term] = field.split(',')

			if len(oline[term]) == 1 and oline[term][0] == '-':
				oline[term] = []

			x += 1
		if len(oline) is not len(self.headers):
			exit("Error at line: " + str(line))
		self.data[oline['name']] = oline

	def removeStarts(self):
		newStart = 'S\''
		oline = {'o': False, 'i': True, 'name': newStart}

		for x in range(3, len(self.headers)):
			oline[self.headers[x]] = []

		for rule in self.data:
			if self.data[rule]['i']:
				oline['eps'].append(rule)
				self.data[rule]['i'] = False

		self.data[newStart] = oline

	def removeEps(self):
		data = {}
		for rule in self.data:
			if rule not in data:
				data[rule] = {'i': self.data[rule]['i'], 'o': self.data[rule]['o'], 'name': self.data[rule]['name']}

			closure = self.epsClosure(rule)
			for termi in self.terms:
				term = self.headers[termi]
				rules = set()
				for clos in closure:
					rules = rules.union(self.data[clos][term])
				data[rule][term] = list(rules)

		eps = [(key, self.headers[key]) for key in self.headers if self.headers[key] == nka.EPS]
		if len(eps) > 0:
			del self.headers[eps[0][0]]
			
		self.data = data

	def epsClosure(self, rule):
		closure = set([rule])
		oldLen = 0
		while oldLen is not len(closure):
			oldLen = len(closure)
			for field in closure:
				closure = closure.union(self.data[field][nka.EPS])
		return closure

	def __str__(self):
		output = '\t'
		fields = []
		for x in range(3, len(self.headers)):
			field = self.headers[x]
			output += '\t' + str(field)
			fields.append(field)
		for line in self.data.values():
			output += '\n'
			if line ['o']:
				output += '<'
			if line['i']:
				output += '>'
			output += '\t' + line['name']
			for field in fields:
				flds = line[field]
				if line[field] == []:
					flds = ['-']
				output += '\t' + ','.join(flds)
		return output

def main():
	if len(os.sys.argv) is not 2:
		print("python convertor.py input.file")
		exit()
	ka = nka(os.sys.argv[1])
	ka.removeStarts()
	ka.removeEps()
	print(ka)

if __name__ == "__main__":
	main()
