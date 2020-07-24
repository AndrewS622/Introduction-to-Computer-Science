-- List all movies released in 2010 and their ratings, in descending order by rating and then alphabetically by title.
SELECT title, rating FROM movies JOIN ratings ON id = movie_id WHERE year = 2010 ORDER BY rating DESC, title;