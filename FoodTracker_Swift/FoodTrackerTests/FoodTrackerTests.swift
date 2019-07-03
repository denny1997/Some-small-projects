//
//  FoodTrackerTests.swift
//  FoodTrackerTests
//
//  Created by csair on 2019/4/13.
//  Copyright © 2019年 SCP Foundation. All rights reserved.
//

import XCTest
@testable import FoodTracker

class FoodTrackerTests: XCTestCase {

    func testMealInitializationSucceeds() {
        let zeroRatingMeal = Meal.init(name: "Zero" , photo: nil, rating: 0)
        XCTAssertNotNil(zeroRatingMeal)
    }

}
