//
//  MealLoginViewController.swift
//  FoodTracker
//
//  Created by csair on 2019/4/18.
//  Copyright © 2019年 SCP Foundation. All rights reserved.
//

import UIKit

class MealLoginViewController: UIViewController {

    @IBOutlet weak var username: UITextField!
    @IBOutlet weak var password: UITextField!
    static let DocumentsDirectory = FileManager().urls(for: .documentDirectory, in: .userDomainMask).first!
    static let URL=DocumentsDirectory.appendingPathComponent("db.data")
    var db: Dictionary<String,String>? = NSMutableDictionary(contentsOfFile: URL.path) as? Dictionary<String, String>
    //var db: Dictionary<String,String>? = [:]
    @IBAction func signUp(_ sender: UIButton) {
        if db == nil {
            db=[:]
        }
        if db![username.text!] != nil {
            let ac = UIAlertController(title: "Warning", message: "Username has been used", preferredStyle: .alert)
            let ok = UIAlertAction(title: "Ok", style: .cancel, handler: nil)
            ac.addAction(ok)
            self.present(ac, animated: true, completion: nil)
        } else {
            db!.updateValue(password.text!, forKey: username.text!)
            NSMutableDictionary(dictionary: db!).write(toFile: MealLoginViewController.URL.path, atomically: true)
            let ac = UIAlertController(title: "Hint", message: "Sigh up successfully", preferredStyle: .alert)
            let ok = UIAlertAction(title: "Ok", style: .cancel, handler: nil)
            ac.addAction(ok)
            self.present(ac, animated: true, completion: nil)
        }
    }
    @IBAction func login(_ sender: UIButton) {
        print(db as Any)
        if db == nil {
            db=[:]
        }
        let pw=db![username.text!]
        if pw==nil {
            let ac = UIAlertController(title: "Warning", message: "Username does not exist", preferredStyle: .alert)
            let ok = UIAlertAction(title: "Ok", style: .cancel, handler: nil)
            ac.addAction(ok)
            self.present(ac, animated: true, completion: nil)
        } else if pw != password.text! {
            let ac = UIAlertController(title: "Warning", message: "Wrong password", preferredStyle: .alert)
            let ok = UIAlertAction(title: "Ok", style: .cancel, handler: nil)
            ac.addAction(ok)
            self.present(ac, animated: true, completion: nil)
        }
    }
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
    }
    

    
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destination.
        // Pass the selected object to the new view controller.
    }
    

}
