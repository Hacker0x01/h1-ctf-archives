//
//  ViewController.swift
//  Level6
//
//  Created by Christopher Thompson on 6/8/17.
//  Copyright © 2017 Uber. All rights reserved.
//

import UIKit

class ViewController: UIViewController {

    @IBOutlet weak var textField: UITextField!
    @IBOutlet weak var button: UIButton!
    var thing: NSMutableDictionary?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        
        let tap: UITapGestureRecognizer = UITapGestureRecognizer(target: self, action: #selector(ViewController.dismissKeyboard))
        view.addGestureRecognizer(tap)
        
        self.thing = setupTree();
    }
    
    func dismissKeyboard() {
        view.endEditing(true)
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

    @IBAction func buttonPressed(_ sender: Any) {
        self.performSegue(withIdentifier: "storyboardSegue", sender: self)
    }
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        collectMachoChecksums(mach_task_self_, getBaseAddress(mach_task_self_));
        let viewController = segue.destination as! DisplaySomeShit
        viewController.someText = encodeString(self.thing, NSString(string: textField.text!) as String) as String?;
    }
}

