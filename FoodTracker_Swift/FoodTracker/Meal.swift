//
//  Meal.swift
//  FoodTracker
//
//  Created by csair on 2019/4/13.
//  Copyright © 2019年 SCP Foundation. All rights reserved.
//

import UIKit
import os.log

class Meal:NSObject, NSCoding {
    var name: String
    var photo: UIImage?
    var rating: Int
    var money: Int?
    var time: Int?
    
    struct PropertyKey {
        static let name = "name"
        static let photo = "photo"
        static let rating = "rating"
        static let money = "money"
        static let time = "time"
    }
    
    static let DocumentsDirectory = FileManager().urls(for: .documentDirectory, in: .userDomainMask).first!
    static let ArchiveURL=DocumentsDirectory.appendingPathComponent("meal")
    
    required convenience init?(coder aDecoder: NSCoder) {
        guard let name = aDecoder.decodeObject(forKey: PropertyKey.name) as? String else {
            return nil
        }
        let photo = aDecoder.decodeObject(forKey: PropertyKey.photo) as? UIImage
        let rating = aDecoder.decodeInteger(forKey: PropertyKey.rating)
        let money = aDecoder.decodeObject(forKey: PropertyKey.money) as? Int
        let time = aDecoder.decodeObject(forKey: PropertyKey.time) as? Int
        self.init(name: name, photo: photo, rating: rating, money: money, time: time)
    }
    
    init?(name: String, photo: UIImage?, rating: Int, money: Int?, time: Int?) {
        guard !name.isEmpty else {
            return nil
        }
        guard (rating >= 0)&&(rating <= 5) else{
            return nil
        }
        self.name=name
        self.photo=photo
        self.rating=rating
        self.money=money
        self.time=time
    }
    
    func encode(with aCoder: NSCoder) {
        aCoder.encode(name,forKey: PropertyKey.name)
        aCoder.encode(photo,forKey: PropertyKey.photo)
        aCoder.encode(rating,forKey: PropertyKey.rating)
        aCoder.encode(money,forKey: PropertyKey.money)
        aCoder.encode(time,forKey: PropertyKey.time)
    }
    
    
}
