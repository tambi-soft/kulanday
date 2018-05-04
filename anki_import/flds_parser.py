
import sqlite3
import codecs
import json

class FldsParser(object):
    
    
    
    def __init__(self):
        self.connection = sqlite3.connect("../../../anki/persian/collection.anki2")
        self.cursor = self.connection.cursor()
        
        result = self.cursor.execute("SELECT flds FROM notes");
        data = result.fetchall()
        
        fobj = open("../../../anki/persian/media")
        media = ""
        for line in fobj:
            media += line
        media_parsed = json.loads(media)
        
        # invert dict:
        media_inv = {v: k for k, v in media_parsed.items()}
        
        for line in data:
            if line[0].find("[sound:") > 1:
                print("")
                
                splitted = line[0].split("\x1f")
                
                #print(splitted)
                if (len(splitted) < 5):
                    #print(splitted)
                    pass
                else:
                    translation = splitted[0]
                    word = splitted[2]
                    phonetic = splitted[3]
                    
                    sound = ""
                    for i, item in enumerate(splitted):
                        if item.find("[sound:") >= 0:
                            sound = item
                    
                    sound = sound.replace("[sound:", "")
                    sound = sound.replace("]", "")
                    audio_filename = media_inv[sound]
                    
                    
                    print(translation, word, phonetic, audio_filename)
                    
                    
                

if __name__ == "__main__":
    c = FldsParser()
