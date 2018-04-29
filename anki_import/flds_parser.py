
import sqlite3

class FldsParser(object):
    
    
    
    def __init__(self):
        self.connection = sqlite3.connect("../../../anki/persian/collection.anki2")
        self.cursor = self.connection.cursor()
        
        result = self.cursor.execute("SELECT flds FROM notes");
        data = result.fetchall()
        
        fobj = open("../../../anki/persian/media")
        
        for line in data:
            if line[0].find("[sound:") > 1:
                splitted = line[0].split("\x1f")
                
                print(splitted)
                """
                try:
                    word = splitted[1]
                    sound = splitted[5]
                    
                    print(word, sound)
                except IndexError:
                    print("shit")
                """

if __name__ == "__main__":
    c = FldsParser()
