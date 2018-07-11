//
//  DisplaySomeShit.swift
//  Level6
//
//  Created by Christopher Thompson on 6/20/17.
//  Copyright © 2017 Uber. All rights reserved.
//


import UIKit

class DisplaySomeShit: UIViewController {
    
    var someText: String?
    @IBOutlet weak var textField: UITextView!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        let tap: UITapGestureRecognizer = UITapGestureRecognizer(target: self, action: #selector(DisplaySomeShit.dismissKeyboard))
        view.addGestureRecognizer(tap)
        
        textField.text = someText
    }
    
    func dismissKeyboard() {
        view.endEditing(true)
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }
    @IBAction func goBack(_ sender: Any) {
        collectMachoChecksums(mach_task_self_, getBaseAddress(mach_task_self_));
        self.dismiss(animated: true, completion: nil)
    }
}
