import UIKit
import os.log

//保存最高分的信息
class highScore: NSObject, NSCoding {
    func encode(with aCoder: NSCoder) {
        aCoder.encode(score, forKey: PropertyKey.score)
    }
    
    required convenience init?(coder aDecoder: NSCoder) {
        
        let score = aDecoder.decodeInteger(forKey: PropertyKey.score)
        
        
        self.init(score:score)
    }
    
    //用户最高分记录
    var score: Int
    
    static let DocumentsDirectory = FileManager().urls(for: .documentDirectory, in: .userDomainMask).first!
    static let ArchiveURL = DocumentsDirectory.appendingPathComponent("highScore")
    
    struct PropertyKey {
        static let score = "highScore"
    }
    
    init?(score: Int) {
        
        guard (score >= 0) else {
            return nil
        }
        
        self.score = score
        
    }
}
