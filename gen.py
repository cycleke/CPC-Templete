# -*- coding: utf-8 -*-

import os


def gen_section(section, path):
    tex = '\section{%s}\n\n' % (section)
    lang = {
        '.cpp' : '[language=c++]',
        '.sh' : '[language=bash]',
        '.java' : '[language=java,tabsize=4]',
    }
    for source in os.listdir(path):
        file_name, extension = os.path.splitext(source)
        try:
            tex += r'\subsection{%s}' % (file_name) + '\n'
            tex += r'\begin{lstlisting}' + '\n'
            tex += open(os.path.join(path, source), 'r').read() + '\n'
            tex += r'\end{lstlisting}' + '\n'
        except:
            pass
    tex += '\n'
    return tex

def main():
    sections = [
        ('Math', 'Math'),
        ('Dynamic Programming', 'Dynamic_Programming'),
        ('Data Structure', 'Data_Structure'),
        ('String', 'String'),
        ('Graph Theory', 'Graph_Theory'),
        ('Computational Geometry', 'Computational_Geometry'),
        ('Java', 'Java'),
        ('Others', 'Others')
    ]
    tex = ''
    for section, path in sections:
        try:
            tex += gen_section(section, path)
        except:
            pass
    head = open('head.tex', 'r').read()
    tex = head % (tex)
    open('main.tex', 'w').write(tex)

if __name__ == '__main__':
    main()
