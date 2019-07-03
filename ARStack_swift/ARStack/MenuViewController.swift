import UIKit
import os.log

class MenuViewController: UIViewController {

    @IBOutlet weak var logoImage: UIImageView!
    
    @IBOutlet weak var highestScoreLabel: UILabel!
    
    @IBOutlet weak var label1: UILabel!
    
    var highestScore = [highScore]()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        Thread.sleep(forTimeInterval: 3.0) //延迟3秒用于加载启动图

        logoImage.image = UIImage(named: "logoImage")
        
        if let savedScore = loadScore() { //加载历史分数记录
            highestScore += savedScore
            print("aaaaaaa")
        }
        else {
            loadInitScore()  //无历史分数记录时初始化操作
            print("fffloading...")
        }

        highestScoreLabel.text = "\(highestScore[0].score)   points"
         //Do any additional setup after loading the view.
    }
    
    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
        if let savedScore = loadScore() {   //刷新历史分数记录
            highestScoreLabel.text = "\(savedScore[0].score)   points"
        }
    }
    

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destination.
        // Pass the selected object to the new view controller.
    }
    */
    
    //无历史分数记录时初始化操作
    private func loadInitScore() {
        
        guard let score = highScore(score: 0) else {
            fatalError("Unable to instantiate score")
        }
        
        highestScore += [score]
        saveScore()
    }
    
    //保存最高分信息
    private func saveScore() {
        let isSuccessfulSave = NSKeyedArchiver.archiveRootObject(highestScore, toFile: highScore.ArchiveURL.path)
        if isSuccessfulSave {
            os_log("Scores successfully saved.", log: OSLog.default, type: .debug)
        } else {
            os_log("Failed to save users...", log: OSLog.default, type: .error)
        }
    }
    
    //读取最高分信息
    private func loadScore() -> [highScore]?  {
        return NSKeyedUnarchiver.unarchiveObject(withFile: highScore.ArchiveURL.path) as? [highScore]
    }

}
