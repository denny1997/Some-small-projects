//
//  MealViewController.swift
//  FoodTracker
//
//  Created by csair on 2019/4/13.
//  Copyright © 2019年 SCP Foundation. All rights reserved.
//

import UIKit
import os.log

class MealViewController: UIViewController,UITextFieldDelegate,UIImagePickerControllerDelegate,UINavigationControllerDelegate {

    @IBOutlet weak var nameTextField: UITextField!
    @IBOutlet weak var photoImageView: UIImageView!
    @IBOutlet weak var ratingControl: RatingControl!
    @IBOutlet weak var priceTextField: UITextField!
    @IBOutlet weak var timeTextField: UITextField!
    @IBOutlet weak var saveButton: UIBarButtonItem!
    var meal: Meal?
    
    @IBAction func cancel(_ sender: UIBarButtonItem) {
        let isPresentingInAddMealMode = presentingViewController is UINavigationController
        if isPresentingInAddMealMode {
            dismiss(animated: true, completion: nil)
        }
        else if let owningNavigationController = navigationController {
            owningNavigationController.popViewController(animated: true)            
        }
        else {
            fatalError("The MealViewController is not inside a navigation controller")
        }
    }
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        super.prepare(for: segue, sender: sender)
        guard let button = sender as? UIBarButtonItem, button===saveButton else {
            os_log("The save button was not pressed, cancelling", log: OSLog.default, type: .debug)
            return
        }
        let name=nameTextField.text ?? ""
        let photo=photoImageView.image
        let rating=ratingControl.rating
        let money=Int(priceTextField.text ?? "")
        let time=Int(timeTextField.text ?? "")
        meal = Meal(name: name, photo: photo, rating: rating, money: money, time: time)
    }
    override func viewDidLoad() {
        super.viewDidLoad()
        nameTextField.delegate=self
        if let meal = meal {
            navigationItem.title=meal.name
            nameTextField.text=meal.name
            if meal.money != nil {
                priceTextField.text = String(meal.money!)
            } else {
                priceTextField.text = ""
            }
            if meal.time != nil {
                timeTextField.text = String(meal.time!)
            } else {
                timeTextField.text = ""
            }
            photoImageView.image=meal.photo
            ratingControl.rating=meal.rating
        }
        updateSaveButtonState()
    }

    func textFieldShouldReturn(_ textField: UITextField) -> Bool {
        textField.resignFirstResponder()
        return true
    }
    
    func textFieldDidEndEditing(_ textField: UITextField) {
        updateSaveButtonState()
        navigationItem.title=textField.text
    }
    
    func textFieldDidBeginEditing(_ textField: UITextField) {
        saveButton.isEnabled=false
    }

    @IBAction func selectImageFromPhotoLibrary(_ sender: UITapGestureRecognizer) {
        nameTextField.resignFirstResponder()
        let imagePickerController = UIImagePickerController()
        imagePickerController.sourceType = .photoLibrary
        imagePickerController.delegate=self
        present(imagePickerController,animated: true,completion: nil)
    }
    
    func imagePickerControllerDidCancel(_ picker: UIImagePickerController) {
        dismiss(animated: true, completion: nil)
    }
    
    func imagePickerController(_ picker: UIImagePickerController, didFinishPickingMediaWithInfo info: [UIImagePickerController.InfoKey : Any]) {
        guard let selectedImage=info[UIImagePickerController.InfoKey.originalImage] as?UIImage else {
            fatalError("Expected a dictionary containing an image, but was provided the following: \(info)")
        }
        photoImageView.image=selectedImage
        dismiss(animated: true, completion: nil)
    }
    
    private func updateSaveButtonState() {
        let text=nameTextField.text ?? ""
        saveButton.isEnabled = !text.isEmpty
    }
}

