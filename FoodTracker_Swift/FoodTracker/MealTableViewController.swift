//
//  MealTableViewController.swift
//  FoodTracker
//
//  Created by csair on 2019/4/14.
//  Copyright © 2019年 SCP Foundation. All rights reserved.
//

import UIKit
import os.log

class MealTableViewController: UITableViewController {

    @IBOutlet weak var sorting: UISegmentedControl!
    var meals = [Meal]()
    var mealsSortedByPrice = [Meal]()
    var mealsSortedByTime = [Meal]()
    var mealSortedByRating = [Meal]()
    override func viewDidLoad() {
        super.viewDidLoad()
        navigationItem.leftBarButtonItem=editButtonItem
        if let savedMeals = loadMeals() {
            meals += savedMeals
        } else {
            loadSampleMeals()
        }
        mealsSortedByPrice = sortedByPrice(meals)
        mealsSortedByTime = sortedByTime(meals)
        mealSortedByRating = sortedByRating(meals)
    }

    // MARK: - Table view data source

    override func numberOfSections(in tableView: UITableView) -> Int {
        return 1
    }

    override func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return meals.count
    }

    let cellIdentifier = "MealTableViewCell"
    override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        guard let cell = tableView.dequeueReusableCell(withIdentifier: cellIdentifier, for: indexPath) as? MealTableViewCell else {
            fatalError("The dequeued cell is not an instance of MealTableViewCell")
        }
        
        //let meal = meals[indexPath.row]
        let meal: Meal
        switch sorting.selectedSegmentIndex {
            case 0:
                meal = meals[indexPath.row]
            case 1:
                meal = mealsSortedByPrice[indexPath.row]
            case 2:
                meal = mealsSortedByTime[indexPath.row]
            case 3:
                meal = mealSortedByRating[indexPath.row]
            default:
                fatalError("Some problem occured when get the index of sorting button")
        }
        
        var price: String
        if meal.money != nil {
            price = String(meal.money!)
        } else {
            price = "NA"
        }
        
        var Time: String
        if meal.time != nil {
            Time = String(meal.time!)
        } else {
            Time = "NA"
        }
        cell.nameLabel.text=meal.name
        cell.priceTextLabel.text = "Price: $"+price
        cell.timeLabel.text = "Time: "+Time+" min"
        cell.photoImageView.image=meal.photo
        cell.ratingControl.rating=meal.rating

        return cell
    }
    

    
    // Override to support conditional editing of the table view.
    override func tableView(_ tableView: UITableView, canEditRowAt indexPath: IndexPath) -> Bool {
        // Return false if you do not want the specified item to be editable.
        return true
    }
    

    
    // Override to support editing the table view.
    override func tableView(_ tableView: UITableView, commit editingStyle: UITableViewCell.EditingStyle, forRowAt indexPath: IndexPath) {
        if editingStyle == .delete {
            // Delete the row from the data source
            switch sorting.selectedSegmentIndex {
            case 0:
                meals.remove(at: indexPath.row)
            case 1:
                meals.remove(at: meals.firstIndex(of: mealsSortedByPrice[indexPath.row])!)
            case 2:
                meals.remove(at: meals.firstIndex(of: mealsSortedByTime[indexPath.row])!)
            case 3:
                meals.remove(at: meals.firstIndex(of: mealSortedByRating[indexPath.row])!)
            default:
                fatalError("Some problem occured when get the index of sorting button")
            }
            mealsSortedByPrice = sortedByPrice(meals)
            mealsSortedByTime = sortedByTime(meals)
            mealSortedByRating = sortedByRating(meals)
            saveMeals()
            tableView.deleteRows(at: [indexPath], with: .fade)
        } else if editingStyle == .insert {
            // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
        }    
    }
    

    /*
    // Override to support rearranging the table view.
    override func tableView(_ tableView: UITableView, moveRowAt fromIndexPath: IndexPath, to: IndexPath) {

    }
    */

    /*
    // Override to support conditional rearranging of the table view.
    override func tableView(_ tableView: UITableView, canMoveRowAt indexPath: IndexPath) -> Bool {
        // Return false if you do not want the item to be re-orderable.
        return true
    }
    */

    
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        super.prepare(for: segue, sender: sender)
        switch(segue.identifier ?? "") {
        case "AddItem":
            os_log("Adding a new meal.",log: OSLog.default, type: .debug)
        case "ShowDetail":
            guard let mealDetailViewController = segue.destination as? MealViewController else {
                fatalError("Unexpected destination: \(segue.destination)")
            }
            guard let selectedMealCell = sender as? MealTableViewCell else {
                fatalError("Unexpected sender: \(String(describing: sender))")
            }
            guard let indexPath = tableView.indexPath(for: selectedMealCell) else {
                fatalError("The selected cell is not being displayed by the table")
            }
            let selectedMeal: Meal?
            switch sorting.selectedSegmentIndex {
            case 0:
                selectedMeal=meals[indexPath.row]
            case 1:
                selectedMeal=mealsSortedByPrice[indexPath.row]
            case 2:
                selectedMeal=mealsSortedByTime[indexPath.row]
            case 3:
                selectedMeal=mealSortedByRating[indexPath.row]
            default:
                fatalError("Some problem occured when get the index of sorting button")
            }
            mealDetailViewController.meal = selectedMeal!
        default:
            fatalError("Unexpected Segue Identifier: \(String(describing: segue.identifier))")
        }
        
    }
    
    
    @IBAction func unwindToMealList(sender: UIStoryboardSegue) {
        if let sourceViewController = sender.source as? MealViewController, let meal = sourceViewController.meal {
            if let selectedIndexPath = tableView.indexPathForSelectedRow {
                switch sorting.selectedSegmentIndex {
                case 0:
                    meals[selectedIndexPath.row]=meal
                case 1:
                    meals[meals.firstIndex(of: mealsSortedByPrice[selectedIndexPath.row])!]=meal
                case 2:
                    meals[meals.firstIndex(of: mealsSortedByTime[selectedIndexPath.row])!]=meal
                case 3:
                    meals[meals.firstIndex(of: mealSortedByRating[selectedIndexPath.row])!]=meal
                default:
                    fatalError("Some problem occured when get the index of sorting button")
                }
                mealsSortedByPrice = sortedByPrice(meals)
                mealsSortedByTime = sortedByTime(meals)
                mealSortedByRating = sortedByRating(meals)
                tableView.reloadData()
                //tableView.reloadRows(at: [selectedIndexPath], with: .none)
            } else {
                //let newIndexPath=IndexPath(row: meals.count, section: 0)
                meals.append(meal)
                mealsSortedByPrice = sortedByPrice(meals)
                mealsSortedByTime = sortedByTime(meals)
                mealSortedByRating = sortedByRating(meals)
                //tableView.insertRows(at: [newIndexPath], with: .automatic)
                tableView.reloadData()
            }
            saveMeals()
        }
    }

    private func loadSampleMeals() {
        let photo1 = UIImage(named: "meal1")
        let photo2 = UIImage(named: "meal2")
        let photo3 = UIImage(named: "meal3")
        
        guard let meal1 = Meal(name: "Breakfast", photo: photo1, rating: 4, money: 50, time: 10) else {
            fatalError("Unable to instantiate breakfast")
        }
        guard let meal2 = Meal(name: "Lunch", photo: photo2, rating: 5, money: 25, time: 20) else {
            fatalError("Unable to instantiate lunch")
        }
        guard let meal3 = Meal(name: "Dinner", photo: photo3, rating: 3, money: 40, time: 15) else {
            fatalError("Unable to instantiate dinner")
        }
        
        meals += [meal1,meal2,meal3]
    }
    
    private func sortedByPrice(_ meals: [Meal])->[Meal] {
        let count = meals.count
        var t = meals
        for i in 1..<count {
            for j in 0..<count-i {
                let a = t[j].money
                let b = t[j+1].money
                var temp: Meal
                if a == nil && b == nil {
                    continue
                }
                else if a == nil {
                    temp=t[j]
                    t[j]=t[j+1]
                    t[j+1]=temp
                }
                else if b == nil {
                    continue
                }
                else {
                    if a!<b! {
                        temp=t[j]
                        t[j]=t[j+1]
                        t[j+1]=temp
                    }
                }
            }
        }
        return t
    }
    
    private func sortedByTime(_ meals: [Meal])->[Meal] {
        let count = meals.count
        var t = meals
        for i in 1..<count {
            for j in 0..<count-i {
                let a = t[j].time
                let b = t[j+1].time
                var temp: Meal
                if a == nil && b == nil {
                    continue
                }
                else if a == nil {
                    temp=t[j]
                    t[j]=t[j+1]
                    t[j+1]=temp
                }
                else if b == nil {
                    continue
                }
                else {
                    if a!<b! {
                        temp=t[j]
                        t[j]=t[j+1]
                        t[j+1]=temp
                    }
                }
            }
        }
        return t
    }
    
    private func sortedByRating(_ meals: [Meal])->[Meal] {
        let count = meals.count
        var t = meals
        for i in 1..<count {
            for j in 0..<count-i {
                let a = t[j].rating
                let b = t[j+1].rating
                var temp: Meal
                if a<b {
                    temp=t[j]
                    t[j]=t[j+1]
                    t[j+1]=temp
                }
            }
        }
        return t
    }
    

    @IBAction func changeSorting(_ sender: UISegmentedControl) {
        tableView.reloadData()
    }
    private func saveMeals() {
        let isSuccessfulSave = NSKeyedArchiver.archiveRootObject(meals, toFile: Meal.ArchiveURL.path)
        if isSuccessfulSave {
            os_log("Meals successfully saved.", log: OSLog.default, type: .debug)
        } else {
            os_log("Failed to save meals...", log: OSLog.default, type: .error)
        }
    }
    
    private func loadMeals() -> [Meal]?  {
        return NSKeyedUnarchiver.unarchiveObject(withFile: Meal.ArchiveURL.path) as? [Meal]
    }
}

