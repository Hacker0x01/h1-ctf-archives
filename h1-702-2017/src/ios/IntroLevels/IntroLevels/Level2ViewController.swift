//
//  Level2ViewController.swift
//  IntroLevels
//
//  Created by Christopher Thompson on 6/8/17.
//  Copyright © 2017 Uber. All rights reserved.
//

import UIKit
import CryptoSwift

class Level2ViewController: UIViewController {
    @IBOutlet weak var textField: UITextField!
    @IBOutlet weak var textView: UITextView!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        let tap: UITapGestureRecognizer = UITapGestureRecognizer(target: self, action: #selector(Level2ViewController.dismissKeyboard))
        
        view.addGestureRecognizer(tap)
    }
    
    func dismissKeyboard() {
        view.endEditing(true)
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }

    @IBAction func buttonTouched(_ sender: Any) {
        let enteredText = textField.text
        
        let num = Int(enteredText!)
        if (enteredText?.lengthOfBytes(using: String.Encoding.utf8))! != 6 || num == nil {
            return
        }
        
        if enteredText!.md5() != "5b6da8f65476a399050c501e27ab7d91" {
            return
        }
        
        do {
            let aes = try AES(key:enteredText! + "1234" + enteredText!, iv: "deadbeefc4febab3")
            let ciphertext = try aes.decrypt([211, 51, 107, 104, 41, 246, 114, 103, 14, 128, 33, 3, 58, 115, 28, 148, 15, 49, 40, 171, 64, 99, 78, 41, 17, 185, 241, 244, 63, 146, 212, 166])
            
            textView.text = String(data: NSData(bytes: ciphertext, length: ciphertext.count) as Data, encoding: .utf8)
        } catch { }
    }
}

