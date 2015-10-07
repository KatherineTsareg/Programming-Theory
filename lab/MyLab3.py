#-*- coding: utf8 -*-
import urllib
import urllib2
import re
import os


list_of_extensions = ['gif', 'bmp', 'jpg', 'jpeg', 'png', 'js', 'css']
urls_list = [] 
#url = raw_input('Write url: ')
#word = raw_input('Write the word: ')
url = 'http://lenta.ru'
word = 'СМИ'
content_txt = open('content.txt','w')
content_txt.write(urllib2.urlopen(url).read())
content_txt.close()
fin = open('content.txt', 'r')
for line in fin:
    if line.find(word) >= 0:
        word_url = url + line[line.find('href="') + 6 : line.rfind('">')]
        if word_url not in urls_list:
            urls_list.append(word_url)
my_dir = os.getcwd()
count = 1
for url in urls_list:
    new_dir = my_dir + '\\' + str(count)
    count += 1;
    os.mkdir(new_dir)
    os.chdir(new_dir)
    content = urllib2.urlopen(url).read()
    img_urls = re.findall('img.*?src="(.*?)"', content)
    js_urls = re.findall('script.*?src="(http:*.*?)"', content)
    css_urls = re.findall('link.*?href=\"(.*?.css)\"',content)
    urls = img_urls + js_urls + css_urls
    for j in range(len(urls)):
        address = urls[j]
        if address[address.rfind('.') + 1 : ] in list_of_extensions:
            name_file = address[address.rfind('/') + 1 : ]
            urllib.urlretrieve(address, name_file)
    fout = open('text_url.html', 'w')
    fout.write(content)
    fout.close()
fin.close()
print 'end'
