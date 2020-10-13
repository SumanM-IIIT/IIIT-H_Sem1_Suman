from bs4 import BeautifulSoup as soup
from urllib.request import urlopen

my_url = "https://www.amazon.in/gp/bestsellers/books/"
url_client = urlopen(my_url)
page_html = url_client.read()
url_client.close()
#print(page_html)

page_soup = soup(page_html, "html.parser")
containers = page_soup.findAll("span", {"class": "aok-inline-block zg-item"})
print(containers[1].prettify())