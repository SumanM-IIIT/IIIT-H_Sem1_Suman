from bs4 import BeautifulSoup as soup
from urllib.request import urlopen
import csv

#my_url = "https://www.amazon.in/gp/bestsellers/books/"
url_list = ["https://www.amazon.in/gp/bestsellers/books/ref=zg_bs_pg_1?ie=UTF8&pg=1", "https://www.amazon.in/gp/bestsellers/books/ref=zg_bs_pg_2?ie=UTF8&pg=2"]
rows = [['Name', 'URL', 'Author', 'Price', 'Number of Ratings', 'Average Ratings\n']]

try:
	for u in url_list:
		url_client = urlopen(u)
		page_html = url_client.read()
		url_client.close()

		page_soup = soup(page_html, "html.parser")
		containers = page_soup.findAll("span", {"class": "aok-inline-block zg-item"})

		for container in containers:
			if not container.find("div", {"class": "p13n-sc-truncate p13n-sc-line-clamp-1 p13n-sc-truncate-desktop-type2"}):
				name = 'Not Available'
			else:
				name = container.find("div", {"class": "p13n-sc-truncate p13n-sc-line-clamp-1 p13n-sc-truncate-desktop-type2"}).string.strip()
			
			if not (container.find('a', attrs = {'class': 'a-link-normal'}))['href']:
				url = 'Not Available'
			else:
				url = 'https://amazon.in' + (container.find('a', attrs = {'class': 'a-link-normal'}))['href']
			
			if not container.find("a", {"class": "a-size-small a-link-child"}):
				author = 'Not Available'
			else:
				author = container.find("a", {"class": "a-size-small a-link-child"}).string.strip()

			if not container.find("span", {"class": "p13n-sc-price"}):
				price = 'Not Available'
			else:
				price = container.find("span", {"class": "p13n-sc-price"}).string.strip()
			
			if not container.find("a", {"class": "a-size-small a-link-normal"}): 
				ratings = 'Not Available'
			else:
				ratings = container.find("a", {"class": "a-size-small a-link-normal"}).string.strip()

			if not container.find("span", {"class": "a-icon-alt"}):
				avg_ratings = 'Not Available'
			else:	
				avg_ratings = container.find("span", {"class": "a-icon-alt"}).string.strip()
			
			demo_list = [name, url, author, price, ratings, avg_ratings + '\n']
			rows.append(demo_list)
		

	with open('output/in_book.csv', 'w') as file:
		writer = csv.writer(file, delimiter=';')
		writer.writerows(rows)

except:
		print('URLLib Exception Handled: Temporary failure in name resolution')
		print('<< PLEASE TRY AGAIN >>')